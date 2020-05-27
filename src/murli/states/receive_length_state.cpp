#ifndef RECEIVELENGTHSTATE_H
#define RECEIVELENGTHSTATE_H

#include "no_mod_state.cpp"
#include "receive_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/centeredTextView.cpp"

namespace Murli
{
    class ReceiveLengthState : public State
    {
        public:
            ReceiveLengthState()
            {
                _centeredTextView = std::make_shared<CenteredTextView>();
                _centeredTextView->setText("Receiving MOD ...");
            }

            void run(MurliContext& context)
            {          
                context.getLed().setColor(Murli::Cyan);
                context.getDisplay().setView(_centeredTextView);
                                
                byte buffer[sizeof(int)] = { 0 };
                Serial.readBytes(buffer, sizeof(int));

                int length = 0;
                length = buffer[3] << 24;
                length += buffer[2] << 16;
                length += buffer[1] << 8;
                length += buffer[0];

                if(length <= ModMemorySize)
                {
                    Serial.write(30);
                    context.currentState = std::make_shared<ReceiveModState>(length);
                }
                else
                {
                    Serial.write(31);
                    context.currentState = std::make_shared<NoModState>();
                }
            }
        
        private:
            std::shared_ptr<CenteredTextView> _centeredTextView;
    };
}

#endif // RECEIVELENGTHSTATE_H