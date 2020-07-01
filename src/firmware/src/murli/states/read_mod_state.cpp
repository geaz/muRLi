#ifndef READMODSTATE_H
#define READMODSTATE_H

#include "broadcast_mod_state.cpp"
#include "invalid_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../views/icon_text_view.cpp"
#include "../../visualization/script_context.hpp"

namespace Murli
{
    class ReadModState : public State
    {
        public:
            ReadModState()
            {
                _readModView = std::make_shared<IconTextView>("Reading MOD ...", u8g2_font_open_iconic_thing_2x_t, 74);
            }

            void run(MurliContext& context)
            {                         
                context.getLed().setAllLeds(Murli::Yellow);
                context.getLed().loop();
                context.getDisplay().setView(_readModView);
                context.getDisplay().loop();

                std::vector<uint8_t> modChars;
                uint8_t result = context.getRom().read(modChars, ModMemorySize);

                if(result == 0 && !isEmptyMod(modChars))
                {
                    std::string loadedMod((char*)&modChars[0]);
                    ScriptContext scriptContext = ScriptContext(context.getLed(), loadedMod);                    
                    if(!scriptContext.isFaulted()) context.currentState = std::make_shared<BroadcastModState>(context, loadedMod);
                    else context.currentState = std::make_shared<InvalidModState>();
                }
                else context.currentState = std::make_shared<InvalidModState>();     
            }

        private:
            bool isEmptyMod(const std::vector<uint8_t>& modChars) const
            {
                // A factory ROM will return the same char for the
                // whole memory - if the mod is empty, it is invalid
                bool isEmptyMod = true;
                for (uint32_t i = 1; i < modChars.size(); i++)
                {
                    if (modChars[i] != modChars[0])
                    {
                        isEmptyMod = false;
                        break;
                    }
                }
                return isEmptyMod;
            }

            std::shared_ptr<IconTextView> _readModView;
    };
}

#endif // READMODSTATE_H