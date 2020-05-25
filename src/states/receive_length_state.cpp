#ifndef RECEIVELENGTHSTATE_H
#define RECEIVELENGTHSTATE_H

#include "state.hpp"
#include "no_mod_state.cpp"
#include "receive_mod_state.cpp"
#include "../led/led.hpp"
#include "../display/centeredTextView.cpp"

namespace Murli
{
    class ReceiveLengthState : public State
    {
        public:
            ReceiveLengthState(LED& led, Display& display) :
                State(led, display),
                _noModState(led, display),
                _receiveModState(led, display) {}

            State* run(StateContext& context)
            {          
                _led.setColor(Murli::Cyan);
                _centeredTextView.setText("Receiving MOD ...");
                _display.setView(&_centeredTextView);
                
                State* nextState = this;
                
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
                    nextState = &_receiveModState;
                    context.receivingLength = length;
                }
                else
                {
                    Serial.write(31);
                    nextState = &_noModState;
                    context.writeRequested = false;

                    _centeredTextView.setText("Error receiving mod (L)!");
                    _display.setView(&_centeredTextView);
                    _led.blink(Murli::Red);
                }

                return nextState;
            }
        
        private:
            NoModState _noModState;
            ReceiveModState _receiveModState;
            CenteredTextView _centeredTextView;
    };
}

#endif // RECEIVELENGTHSTATE_H