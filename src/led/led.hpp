#ifndef LED_H
#define LED_H

#include <memory>
#include <vector>
#include <FastLED.h>
#include "color.hpp"

namespace Murli
{    
    struct LedBlink
    {
        Color color;
        Color lastColor;
        uint8_t speed;
        uint32_t lastBlink;
    };

    class LED
    {    
        public:
            LED();
            
            void loop();
            void setLed(const uint32_t index, const Color color);
            void setAllLeds(Color color);
            void blink(const Color color, const uint8_t speed = 100);

        private:
            void checkBlink();

            std::array<CRGB, LED_COUNT> _leds;
            std::shared_ptr<LedBlink> _blink = NULL;
    };
}

#endif // LED_H