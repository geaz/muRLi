#ifndef LED_H
#define LED_H

#define FASTLED_INTERNAL
#include <array>
#include <vector>
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

            void setGroup(const uint32_t index, std::vector<uint32_t> group);
            void clearGroups();

        private:
            void checkPattern();

            bool _groupsSet = false;
            std::array<CRGB, LED_COUNT> _leds;
            std::shared_ptr<LedPattern> _pattern = nullptr;
            std::array<std::vector<uint32_t>, LED_COUNT> _groups;
    };
}

#endif // LED_H