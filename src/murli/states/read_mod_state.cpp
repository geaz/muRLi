#ifndef READMODSTATE_H
#define READMODSTATE_H

#include "../state.hpp"
#include "../murli_context.hpp"
#include "invalid_mod_state.cpp"
#include "../../display/read_mod_view.cpp"
#include "../../visualization/script_context.hpp"

namespace Murli
{
    class ReadModState : public State
    {
        public:
            ReadModState()
            {
                _readModView = std::make_shared<ReadModView>();
            }

            void run(MurliContext& context)
            {                         
                context.getLed().setColor(Murli::Yellow);
                context.getDisplay().setView(_readModView);
                context.getDisplay().loop();

                std::vector<uint8_t> modChars;
                uint8_t result = context.getRom().read(modChars, ModMemorySize);
                if(result != 0) context.currentState = std::make_shared<InvalidModState>();
                else
                {                        
                    std::string loadedMod((char*)&modChars[0]);

                    // Test run the script to check, if it is valid
                    _scriptContext = std::make_shared<ScriptContext>(loadedMod);
                    if(!_scriptContext->isFaulted()) context.currentState = std::make_shared<InvalidModState>();
                    else context.currentState = std::make_shared<InvalidModState>();
                }             
            }

        private:
            std::shared_ptr<ScriptContext> _scriptContext;
            std::shared_ptr<ReadModView> _readModView;
    };
}

#endif // READMODSTATE_H