#ifndef READMODSTATE_H
#define READMODSTATE_H

#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/centeredTextView.cpp"

namespace Murli
{
    class ReadModState : public State
    {
        public:
            ReadModState()
            {
                _centeredTextView = std::make_shared<CenteredTextView>();
                _centeredTextView->setText("Reading MOD ...");
            }

            void run(MurliContext& context)
            {                         
                context.getLed().setColor(Murli::Yellow);
                context.getDisplay().setView(_centeredTextView);

                /*uint8_t result = context.getRom().read(context.loadedMod, ModMemorySize);
                if(result == 0)
                {
                    Serial.println((char*)context.loadedMod);
                    nextState = &_invalidModState;
                }
                else
                {
                    nextState = &_invalidModState;
                }

                return nextState;*/
            }

        private:
            std::shared_ptr<CenteredTextView> _centeredTextView;
    };
}

#endif // READMODSTATE_H