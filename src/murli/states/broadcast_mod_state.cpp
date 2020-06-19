#ifndef BROADCASTMODSTATE_H
#define BROADCASTMODSTATE_H

#include "run_mod_state.cpp"
#include "invalid_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/views/icon_text_view.cpp"
#include "../../visualization/script_context.hpp"

namespace Murli
{
    class BroadcastModState : public State
    {
        public:
            BroadcastModState(std::string mod) : _mod(mod)
            {
                _broadcastModView = std::make_shared<IconTextView>("Broadcasting MOD ...", u8g2_font_open_iconic_thing_2x_t, 74);
            }

            void run(MurliContext& context)
            {                         
                context.getLed().setAllLeds(Murli::Yellow);
                context.getDisplay().setView(_broadcastModView);
                context.getDisplay().loop();

                if(!_broadcastStarted && context.getMeshLedCount() > LED_COUNT)
                {
                    context.getSocketServer().broadcastMod(_mod);
                    context.getSocketServer().onModDistributed([this]() { _modDistributed = true; });
                    _broadcastStarted = true;
                }
                else if(context.getSocketServer().connectedClients() == 0) _modDistributed = true;

                if(_modDistributed)
                {
                    uint32_t clients = context.getSocketServer().connectedClients();
                    if(clients == ++_receivedCommands || clients == 0)
                    {
                        auto scriptContext = std::make_shared<ScriptContext>(context.getLed(), _mod);
                        context.currentState = std::make_shared<RunModState>(scriptContext);
                    }
                    else _modDistributed = false;
                }
            }

        private:
            std::string _mod;
            std::shared_ptr<IconTextView> _broadcastModView;
            uint32_t _receivedCommands = 0;
            bool _broadcastStarted = false;
            bool _modDistributed = false;
    };
}

#endif // BROADCASTMODSTATE_H