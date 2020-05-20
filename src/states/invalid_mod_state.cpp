#ifndef INVALIDMODSTATE_H
#define INVALIDMODSTATE_H

#include "state.hpp"
#include "../led/led.hpp"
#include "../display/centeredTextView.cpp"

namespace Murli
{
    class InvalidModState : public State
    {
        public:
            InvalidModState(LED& led, Display& display) : State(led, display) {}

            State* run(StateContext& context)
            {
                State* nextState = this;

                _led.setColor(Murli::Red);
                _centeredTextView.setText("Invalid MOD ...");
                _display.setView(&_centeredTextView);

                return nextState;
            }

        private:
            CenteredTextView _centeredTextView;
    };
}

#endif // INVALIDMODSTATE_H