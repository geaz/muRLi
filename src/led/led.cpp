#include "led.hpp"

namespace Murli
{
    LED::LED()
    {
        for(uint8_t index = 0; index < LedCount; index++)
            _leds.push_back(CRGB::Black);

        FastLED
            .addLeds<WS2812B, LedDataPin, GRB>(&_leds[0], LedCount)
            .setCorrection(TypicalLEDStrip);
        FastLED.setTemperature(Candle);
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
        setAllLeds(_currentColor);
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
        setAllLeds(_currentColor);
    }

    Color LED::getColor() const
    {
        Color currentColor;
        currentColor.Red = _leds[0].r;
        currentColor.Green = _leds[0].g;
        currentColor.Blue = _leds[0].b;
        
        return currentColor; 
    }

    void LED::setColor(const Color newColor)
    {
        _currentColor = newColor;
        setAllLeds(_currentColor); 
    }

    void LED::setAllLeds(const Color color)
    {
        for(uint16_t index = 0; index < LedCount; index++)
            _leds[index] = CRGB(color.Red, color.Green, color.Blue);     
        FastLED.show();
    }
}