#pragma once
#ifndef SOUNDVISUALIZATIONCLIENT_H
#define SOUNDVISUALIZATIONCLIENT_H

#include <deque>
#include "../led/led.hpp"

namespace Murli
{    
    static const unsigned char Noise = 35;  // Pretty high noise, because of the LEDs even with the capacitor
    static const unsigned char MaxRMSCount = 100;
    static const unsigned char SampleWindow = 50;
    static const unsigned short FFTDataSize = 256;
    static const unsigned short SampleRate = 10000;
    static const double MinimumMaxRMS = 512 / 3;

    static const Color LowFreqColor = Murli::Blue;
    static const Color MidFreqColor = Murli::Green;
    static const Color HighFreqColor = Murli::Red;

    // https://en.wikipedia.org/wiki/Audio_frequency
    static const short MinFrequency = 130;  // Lowest note for viola, mandola - MinFrequency to trigger color change
    static const short MidFrequency = 1046; // 1046 Highest note reproducible by average female
    static const short MaxFrequency = 3140; // 3140 Between highest note on a flute and on a 88-key piano - Everything above will be full HighFreqColor

    class SoundVisualization
    {
        public:
            Color getSoundColor();

        private:
            double collectSamples();
            unsigned char getVolumeBrightness(const float sampleRMS) const;
            unsigned short getDominantFrequency();
            Color getNewSoundColor(unsigned char volumeBrightness, unsigned short dominantFrequency);

            int16_t fftData[FFTDataSize];
            std::deque<double> lastRMSDeque;
    };
}

#endif // SOUNDVISUALIZATIONCLIENT_H