#ifndef READMODSTATE_H
#define READMODSTATE_H

#include "../state.hpp"
#include "../murli_context.hpp"
#include "invalid_mod_state.cpp"
#include "../../display/read_mod_view.cpp"

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

                uint8_t* mod = new uint8_t[ModMemorySize];
                uint8_t result = context.getRom().read(mod, ModMemorySize);
                if(result == 0)
                {
                    Serial.println((char*)mod);
                    context.currentState = std::make_shared<InvalidModState>();
                }
                else
                {
                    context.currentState = std::make_shared<InvalidModState>();
                }
            }

        private:
            std::shared_ptr<ReadModView> _readModView;
    };
}

#endif // READMODSTATE_H