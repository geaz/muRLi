#ifndef LEDFADEPATTERN_H
#define LEDFADEPATTERN_H

#include "../led.hpp"
#include "../color.hpp"
#include "../led_pattern.hpp"

namespace Murli
{
    class LedFadePattern : public LedPattern
    {
        public:
            LedFadePattern(Color color, uint8_t speed = 50) : _color(color), _speed(speed) { }

            void loop(LED& led) override
            {
                if(_lastFade + _speed < millis())
                {            
                    led.setAllLeds(_color.getFaded(_currentFade));

                    _currentFade += _fadeAmount;
                    if(_currentFade <= 0 || _currentFade >= 255)
                    {
                        _fadeAmount = -_fadeAmount;
                    }            
                    _lastFade = millis();
                }      
            };

        private:
            Color _color;
            uint8_t _speed;
            
            uint8_t _fadeAmount = 5;
            uint8_t _currentFade = 0;
            uint64_t _lastFade = 0;
    };
}

#endif // LEDFADEPATTERN_H