#ifndef NOMODSTATE_H
#define NOMODSTATE_H

#include "state.hpp"
#include "read_mod_state.cpp"
#include "../led/led.hpp"
#include "../display/centeredTextView.cpp"

namespace Murli
{
    class NoModState : public State
    {
        public:
            NoModState(LED& led, Display& display) : 
                State(led, display),
                _readModState(led, display) {}

            State* run(StateContext& context)
            {
                _centeredTextView.setText("Insert MOD ...");
                _display.setView(&_centeredTextView);
                _led.fadeLoop(Murli::Yellow);

                State* nextState = this;
                if(isModInserted())
                {                    
                    _led.stopFadeLoop();
                    _centeredTextView.setText("Reading MOD ...");
                    _display.setView(&_centeredTextView);
                    nextState = &_readModState;
                }
                return nextState;
            }

        private:
            ReadModState _readModState;
            CenteredTextView _centeredTextView;
    };
}

#endif // NOMODSTATE_H