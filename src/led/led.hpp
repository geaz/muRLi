#ifndef LED_H
#define LED_H

#include <vector>
#include <FastLED.h>
#include "color.hpp"

namespace Murli
{    
    static const uint8_t LedCount = 9;
    static const uint8_t LedDataPin = D4;

    class LED
    {    
        public:
            LED();
            
            void blink(const Color color, const uint8_t times = 3);

            void fadeLoop(const Color fadeColor, const uint8_t speed = 50);
            void stopFadeLoop();

            Color getColor() const;
            void setColor(Color color);

        private:
            void setAllLeds(Color color);

            std::vector<CRGB> _leds;
            Color _currentColor = Murli::White;
            uint8_t _fadeAmount = 5;
            uint8_t _currentFade = 0;
            uint64_t _lastFade = 0;
    };
}

#endif // LED_H