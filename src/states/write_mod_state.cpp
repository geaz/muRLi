#ifndef WRITEMODSTATE_H
#define WRITEMODSTATE_H

#include "state.hpp"
#include "invalid_mod_state.cpp"
#include "../led/led.hpp"
#include "../display/centeredTextView.cpp"

namespace Murli
{
    class WriteModState : public State
    {
        public:
            WriteModState(LED& led, Display& display) :
                State(led, display),
                _noModState(led, display) {}

            State* run(StateContext& context)
            {      
                int clearResult = context.rom.clear();
                int result = context.rom.write(context.receivedMod, context.receivingLength);
                if(clearResult == 0 && result == 0)
                {
                    Serial.write(30);

                    _centeredTextView.setText("MOD saved!");
                    _display.setView(&_centeredTextView);
                    _led.blink(Murli::Green);
                }
                else
                {
                    Serial.write(31);

                    _centeredTextView.setText("Error during write!");
                    _display.setView(&_centeredTextView);
                    _led.blink(Murli::Red);
                }
                context.writeRequested = false;
                return &_noModState;
            }
        
        private:
            InvalidModState _noModState;
            CenteredTextView _centeredTextView;
    };
}

#endif // WRITEMODSTATE_H