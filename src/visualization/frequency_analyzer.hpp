#ifndef FREQUENCYANALYZER_H
#define FREQUENCYANALYZER_H

#include <deque>

namespace Murli
{    
    static const unsigned char Noise = 50;  // Pretty high noise, because of the LEDs even with the capacitor
    static const unsigned char MaxRMSCount = 100;
    static const unsigned char SampleWindow = 50;
    static const unsigned short FFTDataSize = 256;
    static const unsigned short SampleRate = 9800;
    static const double MinimumMaxRMS = 512 / 3;

    // https://en.wikipedia.org/wiki/Audio_frequency
    static const short MinFrequency = 130;  // Lowest note for viola, mandola
    static const short MidFrequency = 1046; // 1046 Highest note reproducible by average female
    static const short MaxFrequency = 3140; // 3140 Between highest note on a flute and on a 88-key piano

    class FrequencyAnalyzer
    {
        public:
            void loop();

            uint8_t getVolume();
            uint16_t getDominantFrequency();
            std::vector<uint8_t> getFrequencyRange(uint8_t buckets, uint8_t maxValue);

        private:
            double collectSamples();
            uint8_t calculateVolume(const double sampleRMS) const;
            uint16_t calculateDominantFrequency();
            uint8_t getUpperBoundFreqBucketIndex(uint16_t frequency);

            uint8_t _volume;
            uint16_t _dominantFrequency;
            int16_t fftData[FFTDataSize];
            std::deque<double> lastRMSDeque;
    };
}

#endif // FREQUENCYANALYZER_H