#ifndef FREQUENCYANALYZER_H
#define FREQUENCYANALYZER_H

#include <deque>

namespace Murli
{    
    static const unsigned short FFTDataSize = 128;

    // https://en.wikipedia.org/wiki/Audio_frequency
    static const short MinFrequency = 130;  // Lowest note for viola, mandola
    static const short MidFrequency = 1046; // 1046 Highest note reproducible by average female
    static const short MaxFrequency = 3140; // 3140 Between highest note on a flute and on a 88-key piano

    struct AnalyzerResult
    {
        float decibel;
        uint32_t sampleRate;
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
            void calculateDominantFrequency(AnalyzerResult& result);
    };
}

#endif // FREQUENCYANALYZER_H