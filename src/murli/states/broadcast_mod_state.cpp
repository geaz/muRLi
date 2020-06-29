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
                    context.getSocketServer().addOnCommandReceived([this](Server::Command command) 
                    { 
                        _modDistributed = command.commandType == Server::MOD_DISTRIBUTED; 
                    });
                    _broadcastStarted = true;
                }
                else if(context.getSocketServer().getClientsCount() == 0) _modDistributed = true;

                if(_modDistributed)
                {
                    auto scriptContext = std::make_shared<ScriptContext>(context.getLed(), _mod);
                    scriptContext->updateLedInfo(0, 0, context.getMeshLedCount());
                    scriptContext->init();

                    context.getLed().setAllLeds(Murli::Black);
                    context.currentState = std::make_shared<RunModState>(scriptContext);

                    _modDistributed = false;
                    _broadcastStarted = false;
                }
            }

        private:
            std::string _mod;
            std::shared_ptr<IconTextView> _broadcastModView;
            bool _broadcastStarted = false;
            bool _modDistributed = false;
    };
}

#endif // BROADCASTMODSTATE_H