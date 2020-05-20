#ifndef RECEIVELENGTHSTATE_H
#define RECEIVELENGTHSTATE_H

#include "state.hpp"
#include "no_mod_state.cpp"
#include "../led/led.hpp"
#include "../display/centeredTextView.cpp"

namespace Murli
{
    class ReceiveLengthState : public State
    {
        public:
            ReceiveLengthState(LED& led, Display& display) :
                State(led, display),
                _noModState(led, display) {}

            State* run(StateContext& context)
            {
                State* nextState = this;
                
                byte buffer[sizeof(int)] = { 0 };
                Serial.readBytes(buffer, sizeof(int));

                int length = 0;
                length = buffer[0] << 24;
                length += buffer[1] << 16;
                length += buffer[2] << 8;
                length += buffer[3];

                if(length <= ModMemorySize)
                {
                    Serial.write(30);
                    nextState = &_noModState;
                }
                else
                {
                    Serial.write(31);
                    context.writeRequested = false;
                    nextState = &_noModState;
                }

                return nextState;
            }
        
        private:
            int count = 0;
            NoModState _noModState;
            CenteredTextView _centeredTextView;
    };
}

#endif // RECEIVELENGTHSTATE_H