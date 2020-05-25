#ifndef RECEIVEMODSTATE_H
#define RECEIVEMODSTATE_H

#include "state.hpp"
#include "no_mod_state.cpp"
#include "write_mod_state.cpp"
#include "../led/led.hpp"
#include "../display/centeredTextView.cpp"

namespace Murli
{
    class ReceiveModState : public State
    {
        public:
            ReceiveModState(LED& led, Display& display) :
                State(led, display),
                _noModState(led, display),
                _writeModState(led, display) {}

            State* run(StateContext& context)
            {
                State* nextState = &_noModState;

                if(context.receivingIndex != context.receivingLength && Serial.available())
                {
                    while(Serial.available() > 0)
                    {
                        context.receivedMod[context.receivingIndex++] = Serial.read();
                    }
                    // ACK chunk
                    if(context.receivingIndex % 128 == 0 || context.receivingIndex == context.receivingLength)
                    {
                        Serial.write(30);
                    }
                    nextState = this;
                }
                else if(context.receivingIndex == context.receivingLength)
                {
                    nextState = &_writeModState;

                    _centeredTextView.setText("Writing MOD ...");
                    _display.setView(&_centeredTextView);
                }
                else
                {
                    Serial.write(31);
                    context.writeRequested = false;

                    _centeredTextView.setText("Error receiving mod (M)!");
                    _display.setView(&_centeredTextView);
                    _led.blink(Murli::Red);
                } 
                return nextState;
            }
        
        private:
            NoModState _noModState;
            WriteModState _writeModState;
            CenteredTextView _centeredTextView;
    };
}

#endif // RECEIVEMODSTATE_H