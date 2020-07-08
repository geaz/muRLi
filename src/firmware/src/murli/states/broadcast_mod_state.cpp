#ifndef BROADCASTMODSTATE_H
#define BROADCASTMODSTATE_H

#include "run_mod_state.cpp"
#include "invalid_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../views/icon_text_view.cpp"
#include "../../visualization/script_context.hpp"

namespace Murli
{
    class BroadcastModState : public State
    {
        public:
            BroadcastModState(MurliContext& context, std::string mod)
                : _context(context), _mod(mod)
            {
                _handleId = context.getSocketServer()
                    .serverCommandEvents
                    .addEventHandler([this](Server::Command command) 
                    { 
                        _modDistributed = command.commandType == Server::MOD_DISTRIBUTED; 
                    });
                _broadcastModView = std::make_shared<IconTextView>("Broadcasting MOD ...", u8g2_font_open_iconic_thing_2x_t, 74);
            }

            ~BroadcastModState()
            {
                _context.getSocketServer()
                    .serverCommandEvents
                    .removeEventHandler(_handleId);
            }

            void run(MurliContext& context)
            {                         
                context.getLed().setAllLeds(Murli::Yellow);
                context.getDisplay().setView(_broadcastModView);
                context.getDisplay().loop();

                if(!_broadcastStarted && context.getSocketServer().getClientsCount() > 0)
                {
                    context.getSocketServer().broadcastMod(_mod);
                    _broadcastStarted = true;
                }
                else if(context.getSocketServer().getClientsCount() == 0) _modDistributed = true;

                if(_modDistributed)
                {
                    auto scriptContext = std::make_shared<ScriptContext>(context.getLed(), _mod);
                    scriptContext->updateLedInfo(0, 0, context.getMeshLedCount());
                    scriptContext->init();

                    Client::AnalyzerCommand analyzerCommand = { scriptContext->source };
                    Client::Command command = { millis(), Client::SOURCE_UPDATE };
                    command.analyzerCommand = analyzerCommand;
                    context.getSocketServer().broadcast(command);

                    context.getLed().setAllLeds(Murli::Black);
                    context.currentState = std::make_shared<RunModState>(context, scriptContext);

                    _modDistributed = false;
                    _broadcastStarted = false;
                }
            }

        private:
            MurliContext& _context;
            std::string _mod;
            uint64_t _handleId;

            std::shared_ptr<IconTextView> _broadcastModView;
            bool _broadcastStarted = false;
            bool _modDistributed = false;
    };
}

#endif // BROADCASTMODSTATE_H