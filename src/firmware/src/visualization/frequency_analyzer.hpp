#ifndef FREQUENCYANALYZER_H
#define FREQUENCYANALYZER_H

#include <math.h>
#include <array>

namespace Murli
{    
    // We want recognize 3140hz max - setting the sample rate to 8000 will provide us with bins up to 4000 hz
    // The ESP8266 is able to sample up to a rate of ~10000
    static const uint16_t SampleRate = 9000;
    static const uint64_t SamplePerioduSec = round(1000000*(1.0 / SampleRate)); // micro seconds between to samples
    static const uint16_t FFTDataSize = 128;

    static const float EfAlpha = 0.4f;
    static const int8_t MinDB = -30;

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

        private:
            void collectSamples(AnalyzerResult& result);
            void calculateVolumeAndBandPass(AnalyzerResult& result);
            void calculateDominantFrequency(AnalyzerResult& result);
            float butterBandPass(const float value);
    
            float _lastDominantFrequency = 0;
            std::array<float, 5> _bandPassValues = { { 0, 0, 0, 0, 0 } };
    };
}

#endif // FREQUENCYANALYZER_H