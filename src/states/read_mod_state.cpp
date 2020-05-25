#ifndef READMODSTATE_H
#define READMODSTATE_H

#include "state.hpp"
#include "invalid_mod_state.cpp"
#include "../led/led.hpp"
#include "../rom/rom_24LC32A.hpp"
#include "../display/centeredTextView.cpp"

namespace Murli
{
    class ReadModState : public State
    {
        public:
            ReadModState(LED& led, Display& display) :
                State(led, display),
                _invalidModState(led, display) {}

            State* run(StateContext& context)
            {                         
                _led.setColor(Murli::Yellow);
                _centeredTextView.setText("Reading MOD ...");
                _display.setView(&_centeredTextView);

                State* nextState = this;
                uint8_t result = context.rom.read(context.loadedMod, ModMemorySize);
                if(result == 0)
                {
                    Serial.println((char*)context.loadedMod);
                    nextState = &_invalidModState;
                }
                else
                {
                    nextState = &_invalidModState;
                }

                return nextState;
            }

        private:
            //RunModState _runModState;
            InvalidModState _invalidModState;
            CenteredTextView _centeredTextView;
    };
}

#endif // READMODSTATE_H