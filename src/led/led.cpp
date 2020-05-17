#include "led.hpp"

namespace Murli
{
    LED::LED()
    {
        for(int index = 0; index < LedCount; index++)
            leds.push_back(CRGB::Black);

        FastLED
            .addLeds<WS2812B, DataPin, GRB>(&leds[0], LedCount)
            .setCorrection(TypicalLEDStrip);
        FastLED.setTemperature(Candle);
    }

    Color LED::getColor() const
    {
        Color currentColor;
        currentColor.Red = leds[0].r;
        currentColor.Green = leds[0].g;
        currentColor.Blue = leds[0].b;
        
        return currentColor; 
    }

    void LED::setColor(const Color newColor)
    {
        currentColor = newColor;
        setAllLeds(currentColor); 
    }

    void LED::blink(const Color blinkColor, const char times)
    {
        for(uint16_t blinkCount = 0; blinkCount < times; blinkCount++)
        {        
            setAllLeds(blinkColor);      
            delay(200);

            setAllLeds(Murli::Black);    
            delay(200);
        }
        setAllLeds(currentColor);
    }

    void LED::setAllLeds(const Color color)
    {
        for(uint16_t index = 0; index < LedCount; index++)
            leds[index] = CRGB(color.Red, color.Green, color.Blue);     
        FastLED.show();
    }
}