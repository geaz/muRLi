#ifndef READMODSTATE_H
#define READMODSTATE_H

#include "run_mod_state.cpp"
#include "invalid_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/views/read_mod_view.cpp"
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
                context.getLed().setAllLeds(Murli::Yellow);
                context.getDisplay().setView(_readModView);
                context.getDisplay().loop();

                std::vector<uint8_t> modChars;
                uint8_t result = context.getRom().read(modChars, ModMemorySize);

                if(result == 0 && !isEmptyMod(modChars))
                {
                    std::string loadedMod((char*)&modChars[0]);
                    auto scriptContext = std::make_shared<ScriptContext>(context.getLed(), loadedMod);                    
                    if(!scriptContext->isFaulted()) context.currentState = std::make_shared<RunModState>(scriptContext);
                    else context.currentState = std::make_shared<InvalidModState>();
                }
                else context.currentState = std::make_shared<InvalidModState>();     
            }

        private:
            bool isEmptyMod(std::vector<uint8_t>& modChars)
            {
                // A factory ROM will return the same char for the
                // whole memory - if the mod is empty, it is invalid
                bool isEmptyMod = true;
                for (int i = 1; i < modChars.size(); i++)
                {
                    if (modChars[i] != modChars[0])
                    {
                        isEmptyMod = false;
                        break;
                    }
                }
                return isEmptyMod;
            }

            std::shared_ptr<ReadModView> _readModView;
    };
}

#endif // READMODSTATE_H