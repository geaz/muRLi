#ifndef FREQUENCYANALYZER_H
#define FREQUENCYANALYZER_H

#include <math.h>
#include <array>
#include "../shared.hpp"

namespace Murli
{   
    static const float EfAlpha = 0.4f;
    static const int8_t MinDB = -35;

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
    
            float _lastDominantFrequency = 0;
            BandPassFilter _bandPassFilter;
    };
}

#endif // FREQUENCYANALYZER_H