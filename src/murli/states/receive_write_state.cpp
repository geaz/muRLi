#ifndef RECEIVEWRITESTATE_H
#define RECEIVEWRITESTATE_H

#include "no_mod_state.cpp"
#include "receive_length_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"

namespace Murli
{
    class ReceiveWriteState : public State
    {
        public:
            void run(MurliContext& context)
            {                
                int incomingByte = Serial.read();
                if(incomingByte == 30)
                {
                    Serial.write(30);
                    context.writeRequested = true;
                    context.currentState = std::make_shared<ReceiveLengthState>();
                }
                else
                {
                    context.currentState = std::make_shared<NoModState>();
                }
            }
    };
}

#endif // RECEIVEWRITESTATE_H