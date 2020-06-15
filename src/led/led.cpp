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
        FastLED.show();
    }
    
    void LED::setLed(const uint32_t index, const Color color)
    {
        _leds[index] = color;
    }

    void LED::setAllLeds(const Color color)
    {
        for(uint16_t index = 0; index < _leds.size(); index++)
            _leds[index] = CRGB(color.Red, color.Green, color.Blue);
    }

    void LED::blink(const Color blinkColor, const uint8_t times)
    {
        for(uint8_t blinkCount = 0; blinkCount < times; blinkCount++)
        {        
            setAllLeds(blinkColor);
            delay(200);

            setAllLeds(Murli::Black);
            delay(200);
        }
    }

    void LED::fadeLoop(Color startColor, const uint8_t speed)
    {
        if(_lastFade + speed < millis())
        {            
            setAllLeds(startColor.getFaded(_currentFade));

            _currentFade += _fadeAmount;
            if(_currentFade <= 0 || _currentFade >= 255)
            {
                _fadeAmount = -_fadeAmount;
            }            
            _lastFade = millis();
        }      
    }   

    void LED::stopFadeLoop()
    {
        _lastFade = 0;
        _fadeAmount = 5;
        _currentFade = 0;
    }
}