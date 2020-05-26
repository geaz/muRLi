#ifndef STATE_H
#define STATE_H

#include <Arduino.h>
#include "../led/led.hpp"
#include "../display/view.hpp"
#include "../display/display.hpp"
#include "../rom/rom_24LC32A.hpp"

namespace Murli
{
    class MurliContext;
    class State
    {
        public:
            virtual void run(MurliContext& context) = 0;
    };
}

#endif