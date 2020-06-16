#ifndef LEDBLINKPATTERN_H
#define LEDBLINKPATTERN_H

#include "../led.hpp"
#include "../color.hpp"
#include "../led_pattern.hpp"

namespace Murli
{
    class LedBlinkPattern : public LedPattern
    {
        public:
            LedBlinkPattern(Color color, int8_t times = -1, uint8_t speed = 200) :
                _color(color), _times(times), _speed(speed) { }

            void loop(LED& led) override
            {
                if(_times == 0)
                {                    
                    led.setAllLeds(Murli::Black);
                    led.setPattern(nullptr);
                }
                else if(_lastUpdate + _speed < millis())
                {
                    Color nextColor = Murli::Black;
                    if(_lastColor == Murli::Black) nextColor = _color;

                    led.setAllLeds(nextColor);
                    _lastColor = nextColor;
                    _lastUpdate = millis();

                    if(_times > 0) _times--;
                }
            };

        private:
            Color _color;
            int8_t _times;
            uint8_t _speed;
            
            Color _lastColor = Murli::Black;
            uint64_t _lastUpdate = 0;
    };
}

#endif // LEDBLINKPATTERN_H