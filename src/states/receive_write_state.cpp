#ifndef RECEIVEWRITESTATE_H
#define RECEIVEWRITESTATE_H

#include "state.hpp"
#include "no_mod_state.cpp"
#include "receive_length_state.cpp"
#include "../led/led.hpp"

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

                    context.writeRequested = true;
                    context.receivingIndex = 0;
                    context.receivingLength = 0;
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
    };
}

#endif // RECEIVEWRITESTATE_H