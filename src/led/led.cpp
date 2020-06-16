#include "led.hpp"

namespace Murli
{
    LED::LED()
    {
        for(uint8_t index = 0; index < _leds.size(); index++)
            _leds[index] = CRGB::Black;

        FastLED
            .addLeds<WS2812B, LED_PIN, GRB>(_leds.data(), _leds.size())
            .setCorrection(TypicalLEDStrip);
        FastLED.setTemperature(Candle);
        FastLED.show();
    }

    void LED::loop()
    {   
        checkBlink();
        FastLED.show();
    }
    
    void LED::setLed(const uint32_t index, const Color color)
    {
        _blink = NULL;
        _leds[index] = color;
    }

    void LED::setAllLeds(const Color color)
    {
        for(uint16_t index = 0; index < _leds.size(); index++)
            _leds[index] = CRGB(color.Red, color.Green, color.Blue);
    }

    void LED::blink(const Color color, const uint8_t speed)
    {
        _blink = std::make_shared<LedBlink>();
        _blink->color = color;
        _blink->lastColor = color;
        _blink->speed = speed;
        _blink->lastBlink = 0;
    }

    void LED::checkBlink()
    {
        if(_blink != NULL && _blink->lastBlink + _blink->speed < millis())
        {
            if(_blink->lastColor == Murli::Black)
            {
                setAllLeds(_blink->color);
            }
            else setAllLeds(Murli::Black);
            _blink->lastBlink = millis();          
        }
    }
}