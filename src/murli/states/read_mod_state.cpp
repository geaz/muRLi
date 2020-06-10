#ifndef READMODSTATE_H
#define READMODSTATE_H

#include "../state.hpp"
#include "../murli_context.hpp"
#include "run_mod_state.cpp"
#include "invalid_mod_state.cpp"
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
                context.getLed().setColor(Murli::Yellow);
                context.getDisplay().setView(_readModView);
                context.getDisplay().loop();

                std::vector<uint8_t> modChars;
                uint8_t result = context.getRom().read(modChars, ModMemorySize);
                if(result != 0) context.currentState = std::make_shared<InvalidModState>();
                else
                {                        
                    std::string loadedMod((char*)&modChars[0]);
                    bool isEmptyMod = true;
                    for (int i = 1; i < loadedMod.length(); i++) 
                        if (loadedMod[i] != loadedMod[0])
                            isEmptyMod = false; 

                    if(isEmptyMod) context.currentState = std::make_shared<InvalidModState>();
                    else
                    {
                        auto scriptContext = std::make_shared<ScriptContext>(loadedMod);                    
                        if(!scriptContext->isFaulted()) context.currentState = std::make_shared<RunModState>(scriptContext);
                        else context.currentState = std::make_shared<InvalidModState>();
                    }
                }             
            }

        private:
            std::shared_ptr<ReadModView> _readModView;
    };
}

#endif // READMODSTATE_H