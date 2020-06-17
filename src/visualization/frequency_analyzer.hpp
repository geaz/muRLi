#ifndef FREQUENCYANALYZER_H
#define FREQUENCYANALYZER_H

#include <array>
#include <vector>

namespace Murli
{    
    // We want recognize 3140hz max - setting the sample rate to 8000 will provide us with bins up to 4000 hz
    // The ESP8266 is able to sample up to a rate of ~10000
    static const uint16_t SampleRate = 8000;
    static const uint64_t SamplePerioduSec = round(1000000*(1.0 / SampleRate)); // micro seconds between to samples
    static const unsigned short FFTDataSize = 128;

    // https://en.wikipedia.org/wiki/Audio_frequency
    static const short MinFrequency = 130;  // Lowest note for viola, mandola
    static const short MidFrequency = 1046; // 1046 Highest note reproducible by average female
    static const short MaxFrequency = 3140; // 3140 Between highest note on a flute and on a 88-key piano
    static const int8_t MinDB = -21;

    struct AnalyzerResult
    {
        float decibel;
        uint8_t volume;
        uint16_t dominantFrequency;
        float fftReal[FFTDataSize];
        float fftImg[FFTDataSize];
    };

    class FrequencyAnalyzer
    {
        public:
            AnalyzerResult loop();
            std::vector<uint8_t> getFrequencyRange(const AnalyzerResult& result, const uint8_t buckets, const uint8_t maxValue) const;

        private:
            void collectSamples(AnalyzerResult& result);
            void calculateVolumeAndBandPass(AnalyzerResult& result);
            void calculateDominantFrequency(AnalyzerResult& result);
            float butterBandPass(const float value);
    
            std::array<float, 5> _bandPassValues = { { 0, 0, 0, 0, 0 } };
    };
}

#endif // FREQUENCYANALYZER_H