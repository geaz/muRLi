#ifndef COLOR_H
#define COLOR_H

#include <FastLED.h>

namespace Murli
{
    struct Color
    {
        uint8_t Red;
        uint8_t Green;
        uint8_t Blue;

        static Color fromHex(const uint32_t hexColor)
        {
            CRGB color = CRGB(hexColor);
            return { color.r, color.g, color.b };  
        }

        static Color blend(const Color color1, const Color color2)
        {
            uint8_t blendedRed = (color1.Red + color2.Red) / 2;
            uint8_t blendedGreen = (color1.Green + color2.Green) / 2;
            uint8_t blendedBlue = (color1.Blue + color2.Blue) / 2;
            return { blendedRed, blendedGreen, blendedBlue };
        }

        Color getFaded(const uint8_t value) const
        { 
            CRGB fadedColor = CRGB(Red, Green, Blue).fadeLightBy(value);
            return { fadedColor.r, fadedColor.g, fadedColor.b };  
        }

        Color getFadedBlack(const uint8_t value) const
        { 
            CRGB fadedColor = CRGB(Red, Green, Blue).fadeToBlackBy(value);
            return { fadedColor.r, fadedColor.g, fadedColor.b };  
        }

        std::string toString() const
        {
            std::array<char, 20> colorString;
            std::sprintf(colorString.begin(), "%i,%i,%i", Red, Green, Blue);
            return std::string(colorString.begin());
        }

        bool isBlack() { return Red == 0 && Green == 0 && Blue == 0; }
        bool operator!=(const Color other) { return other.Red != Red || other.Green != Green || other.Blue != Blue; }
        bool operator==(const Color other) { return other.Red == Red || other.Green == Green || other.Blue == Blue; }
    };
    
    struct ColorFrame
    {
        Color first;
        Color second;
    };

    static const Color Black = { 0, 0, 0 };
    static const Color Red = { 255, 0, 0 };
    static const Color Green = { 0, 255, 0 };
    static const Color Blue = { 0, 0, 255 };
    static const Color Yellow = { 255, 255, 0 };
    static const Color White = { 255, 255, 255 };
    static const Color Cyan = { 0, 255, 255 };
}

#endif // COLOR_H