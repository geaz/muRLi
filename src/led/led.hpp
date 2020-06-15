#ifndef LED_H
#define LED_H

#include <vector>
#include <FastLED.h>
#include "color.hpp"

namespace Murli
{    
    class LED
    {    
        public:
            LED();
            
            void loop();
            void setLed(const uint32_t index, const Color color);
            void setAllLeds(Color color);

            void blink(const Color color, const uint8_t times = 3);
            void fadeLoop(const Color fadeColor, const uint8_t speed = 50);
            void stopFadeLoop();

        private:
            std::array<CRGB, LED_COUNT> _leds;

            uint8_t _fadeAmount = 5;
            uint8_t _currentFade = 0;
            uint64_t _lastFade = 0;
    };
}

#endif // LED_H