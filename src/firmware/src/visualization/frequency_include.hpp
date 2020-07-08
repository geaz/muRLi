#ifndef FREQUENCYINCLUDE_H
#define FREQUENCYINCLUDE_H

#include <array>

namespace Murli
{
    // https://en.wikipedia.org/wiki/Audio_frequency
    static const short MinFrequency = 130;  // Lowest note for viola, mandola
    static const short MidFrequency = 1046; // 1046 Highest note reproducible by average female
    static const short MaxFrequency = 3140; // 3140 Between highest note on a flute and on a 88-key piano

    inline float map(float x, float in_min, float in_max, float out_min, float out_max) {
        return (x - in_min) / (in_max - in_min) * (out_max - out_min) + out_min;
    }

    /**
     * @brief This method returns a consollidated reprensentation of the calculated frequency buckets for the display.
     * 
     * @param amplitudes  The Amplitudes to calculate the frequency range for the display
     * @param sampleRate  The used sample rate to calculate the FFT
     * @param fftDataSize The used data size to calculate the FFT
     * @return std::array<uint8_t, BAR_COUNT> Returns a array with the mapped value of each bucket
     */
    inline std::array<uint8_t, BAR_COUNT> getFrequencyBars(const float* amplitudes, const uint16_t sampleRate, const uint16_t fftDataSize)
    {
        std::array<uint8_t, BAR_COUNT> combinedBuckets;
        
        // only check buckets between our min and max frequency
        uint8_t minFreqIndex = (uint8_t)((float)MinFrequency / ((float)sampleRate / (float)fftDataSize));
        uint8_t maxFreqIndex = (uint8_t)((float)MaxFrequency / ((float)sampleRate / (float)fftDataSize));

        float originalMaxValue = 0;
        for(uint8_t i = minFreqIndex; i < maxFreqIndex; i++)
        {
            originalMaxValue = std::max(originalMaxValue, amplitudes[i]);
        }
        
        uint8_t splitSize = (maxFreqIndex - minFreqIndex + 1) / BAR_COUNT;
        for(uint8_t i = 0; i < BAR_COUNT; i++)
        {
            float combinedValue = 0;
            for(uint8_t j = 0; j < splitSize; j++)
            {
                combinedValue += amplitudes[j + (i * splitSize) + minFreqIndex /* offset */];
            }
            combinedBuckets[i] = (uint8_t) Murli::map(combinedValue/splitSize, 0, originalMaxValue, 0, BAR_HEIGHT);
        }
        return combinedBuckets;
    }
}

#endif // FREQUENCYINCLUDE_H