#pragma once
#ifndef COLOR_H
#define COLOR_H

#include <FastLED.h>

namespace Murli
{
    struct Color
    {
        unsigned char Red;
        unsigned char Green;
        unsigned char Blue;

        Color getFaded(const char value) 
        { 
            CRGB fadedColor = CRGB(Red, Green, Blue).fadeToBlackBy(value);
            return { fadedColor.r, fadedColor.g, fadedColor.b };  
        }

        bool isBlack() { return Red == 0 && Green == 0 && Blue == 0; }
        bool operator!=(const Color other) { return other.Red != Red || other.Green != Green || other.Blue != Blue; }
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