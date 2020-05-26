#ifndef MURLICONTEXT_H
#define MURLICONTEXT_H

#include <memory>

#include "state.hpp"
#include "../led/led.hpp"
#include "../display/display.hpp"
#include "../rom/rom_24LC32A.hpp"

namespace Murli
{
    class MurliContext
    {
        public:
            MurliContext(Display& display, LED& led);

            void loop();
            bool isModInserted() const;
            LED& getLed();
            Rom24LC32A& getRom();
            Display& getDisplay();
            void setState(std::shared_ptr<State> state);

            bool writeRequested = false;

        private:
            LED _led;
            Display _display;
            Rom24LC32A _rom = Rom24LC32A(0x50);

            std::shared_ptr<State> _currentState;
    };
}

#endif // MURLICONTEXT_H