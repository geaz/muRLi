#ifndef STATE_H
#define STATE_H

#include <Arduino.h>
#include "../led/led.hpp"
#include "../display/display.hpp"
#include "../rom/rom_24LC32A.hpp"

namespace Murli
{
    struct StateContext
    {
        Rom24LC32A rom = Rom24LC32A(0x50);
        uint8_t loadedMod[ModMemorySize] = { 0 };
        uint8_t receivedMod[ModMemorySize] = { 0 };
        uint16_t receivingLength, receivingIndex = 0;
        bool writeRequested = false;
    };

    class State
    {
        public:
            State(LED& led, Display& display) :
                _led(led),
                _display(display) {}

            bool isModInserted() const
            {
                return digitalRead(D6) == HIGH;
            }

            virtual State* run(StateContext& context) = 0;

        protected:
            LED& _led;
            Display& _display;
    };
}

#endif