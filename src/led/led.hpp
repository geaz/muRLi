#ifndef LED_H
#define LED_H

#define FASTLED_INTERNAL
#include <array>
#include <memory>
#include <FastLED.h>
#include "led_pattern.hpp"
#include "color.hpp"

namespace Murli
{
    class LED
    {    
        public:
            LED();
            
            void loop();
            void setLed(const uint32_t index, const Color color);
            void setAllLeds(const Color color);
            void setPattern(std::shared_ptr<LedPattern> pattern);

        private:
            void checkPattern();

            std::array<CRGB, LED_COUNT> _leds;
            std::shared_ptr<LedPattern> _pattern = nullptr;
    };
}

#endif // LED_H