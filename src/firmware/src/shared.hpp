#ifndef FREQUENCYINCLUDE_H
#define FREQUENCYINCLUDE_H

#include <array>

/**
 * @brief   This header file includes shared consts and functions for muRLi
 *          and any external source application. Like the desktop application for example.
 */
namespace Murli
{
    // We want recognize 4186hz max - setting the sample rate to 9000 will provide us with bins up to 4500hz
    // The ESP8266 is able to sample up to a rate of ~10000
    static const uint16_t SampleRate = 9000;
    static const uint64_t SamplePerioduSec = (uint64_t) round(1000000*(1.0 / SampleRate)); // micro seconds between two samples
    static const uint16_t FFTDataSize = 128;

    // https://en.wikipedia.org/wiki/Audio_frequency
    static const short MaxFrequency = 4186; // Highest note on a standard 88-key piano.
    static const uint8_t MaxFreqBinIndex = (uint8_t)((float)MaxFrequency / ((float)SampleRate / (float)FFTDataSize));

    inline float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
        return (x - in_min) / (in_max - in_min) * (out_max - out_min) + out_min;
    }

    /**
     * @brief This method returns a consollidated reprensentation of the calculated frequency buckets for the display.
     * 
     * @param amplitudes  The Amplitudes to calculate the frequency range for the display
     * @param volume      The current volume, used to make the bar height a bit more dynamic
     * @return std::array<uint8_t, BAR_COUNT> Returns a array with the mapped value of each bucket
     */
    inline std::array<uint8_t, BAR_COUNT> getFrequencyBars(const float* amplitudes, uint8_t volume)
    {
        std::array<uint8_t, BAR_COUNT> combinedBuckets;
        std::array<float, BAR_COUNT> combinedValues;

        float originalMaxValue = 0.0f;
        uint8_t splitSize = MaxFreqBinIndex / BAR_COUNT;
        for(uint8_t i = 0; i < BAR_COUNT; i++)
        {
            combinedValues[i] = 0.0f;
            for(uint8_t j = 0; j < splitSize; j++)
            {
                combinedValues[i] += amplitudes[j + (i * splitSize)];
            }
            combinedValues[i] /= splitSize;
            originalMaxValue = combinedValues[i] > originalMaxValue
                ? combinedValues[i]
                : originalMaxValue;
        }

        float volumeMultiplier = volume < 50
            ? 0.5f
            : volume / 100.0f;
        for(uint8_t i = 0; i < BAR_COUNT; i++)
        {
            combinedBuckets[i] = (uint8_t) (Murli::mapf(combinedValues[i], 0, originalMaxValue, 0, BAR_HEIGHT) * volumeMultiplier);
        }
        return combinedBuckets;
    }

    /**
     * @brief Bandpass filter for frequencies <= 100 and >= 41000
     * 
     */
    class BandPassFilter
    {
        public:
            // http://www.schwietering.com/jayduino/filtuino/index.php?
            // characteristic=bu&passmode=bp&order=2&usesr=usesr&sr=9000&
            // frequencyLow=100&noteLow=&frequencyHigh=4100&noteHigh=&pw=pw&calctype=float&run=Send
            float filter(const float sample)
            {
                _filterValues[0] = _filterValues[1];
                _filterValues[1] = _filterValues[2];
                _filterValues[2] = _filterValues[3];
                _filterValues[3] = _filterValues[4];
                _filterValues[4] = (7.809958825730578535e-1f * sample)
				 + (-0.61050725513825743196f * _filterValues[0])
				 + (-0.17599206727882477086f * _filterValues[1])
				 + (1.47818833581520703291f * _filterValues[2])
				 + (0.29292247730559561880f * _filterValues[3]);
                return (_filterValues[0] + _filterValues[4]) - 2 * _filterValues[2];
            }
        private:
            std::array<float, 5> _filterValues = { { 0, 0, 0, 0, 0 } };
    };
}

#endif // FREQUENCYINCLUDE_H