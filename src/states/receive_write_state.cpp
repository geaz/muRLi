#ifndef RECEIVEWRITESTATE_H
#define RECEIVEWRITESTATE_H

#include "state.hpp"
#include "no_mod_state.cpp"
#include "receive_length_state.cpp"
#include "../led/led.hpp"
#include "../display/centeredTextView.cpp"

namespace Murli
{
    class ReceiveWriteState : public State
    {
        public:
            ReceiveWriteState(LED& led, Display& display) :
                State(led, display),
                _noModState(led, display),
                _receiveLengthState(led, display) {}

            State* run(StateContext& context)
            {
                State* nextState = this;
                
                int incomingByte = Serial.read();
                if(incomingByte == 30)
                {
                    Serial.write(30);
                    
                    _led.setColor(Murli::Cyan);
                    _centeredTextView.setText("Receiving MOD ...");
                    _display.setView(&_centeredTextView);

                    context.writeRequested = true;
                    nextState = &_receiveLengthState;
                }
                else
                {
                    nextState = &_noModState;
                }          

                return nextState;
            }
        
        private:
            NoModState _noModState;
            ReceiveLengthState _receiveLengthState;
            CenteredTextView _centeredTextView;
    };
}

#endif // RECEIVEWRITESTATE_H