#pragma once
#ifndef LED_H
#define LED_H

#include <vector>
#include <FastLED.h>
#include "color.hpp"

namespace Murli
{    
    static const char LedCount = 8;
    static const char LedDataPin = D4;

    class LED
    {    
        public:
            LED();
            
            void blink(const Color color, const char times = 3);

            void fadeLoop(const Color fadeColor, const char speed = 50);
            void stopFadeLoop();

            Color getColor() const;
            void setColor(Color color);

        private:
            void setAllLeds(Color color);

            std::vector<CRGB> _leds;
            Color _currentColor = Murli::White;
            int _fadeAmount = 5;
            int _currentFade = 0;
            unsigned long _lastFade = 0;
    };
}

#endif // LED_H