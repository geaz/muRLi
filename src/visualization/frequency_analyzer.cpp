#include <arduinoFFT.h>
#include "frequency_analyzer.hpp"

namespace Murli
{
    /**
     * @brief   On each call this method collects samples and analyzes the volume and frequency.
     *          The results are returned in the 'AnalyzerResult' struct.
     */
    AnalyzerResult FrequencyAnalyzer::loop()
    {
        AnalyzerResult result;
        collectSamples(result);                 
        calculateDominantFrequency(result);

        return result;
    }    

    /**
     * @brief This method returns a consollidated reprensentation of the calculated frequency buckets.
     * 
     * @param AnalyzerResult The AnalyzerResult to calculate the frequency range
     * @param buckets Defines into how many buckets the data should be combined
     * @param maxValue Defines the new max value for each bucket. The values will be mapped into this new range (0-maxValue)
     * @return std::vector<uint8_t> Returns a vector with the mapped value of each bucket
     */
    std::vector<uint8_t> FrequencyAnalyzer::getFrequencyRange(const AnalyzerResult& result, const uint8_t buckets, const uint8_t maxValue) const
    {
        std::vector<uint8_t> combinedBuckets;
        
        // only check buckets between our min and max frequency
        uint8_t minFreqIndex = (uint8_t)((float)MinFrequency / ((float)result.sampleRate / (float)FFTDataSize));
        uint8_t maxFreqIndex = (uint8_t)((float)MaxFrequency / ((float)result.sampleRate / (float)FFTDataSize));

        float originalMaxValue = 0;
        for(uint8_t i = minFreqIndex; i < maxFreqIndex; i++)
        {
            originalMaxValue = max(originalMaxValue, result.fftReal[i]);
        }
        
        uint8_t splitSize = (maxFreqIndex - minFreqIndex + 1) / buckets;
        for(uint8_t i = 0; i < buckets; i++)
        {
            uint32_t combinedValue = 0;
            for(uint8_t j = 0; j < splitSize; j++)
            {
                combinedValue += result.fftReal[j + (i * splitSize) + minFreqIndex /* offset */];
            }
            
            uint8_t newMaxValue = map(combinedValue/splitSize, 0, originalMaxValue, 0, maxValue);
            combinedBuckets.push_back(newMaxValue);
        }
        return combinedBuckets;
    }

    void FrequencyAnalyzer::collectSamples(AnalyzerResult& result)
    {
        unsigned long signalRMS = 0;
        unsigned long startMillis = millis();    

        for (int i = 0; i < FFTDataSize; i++)
        {
            unsigned short sample = analogRead(A0);
            result.fftReal[i] = sample;
            result.fftImg[i] = 0;

            sample = abs(sample - 512);
            signalRMS += sample * sample;
        }
        result.sampleRate = FFTDataSize * 1000 / (millis() - startMillis);

        float signalRMSflt = sqrt(signalRMS / FFTDataSize);
        result.decibel = 20.0*log10(signalRMSflt / 512);
        result.volume = map(result.decibel < MinDB ? MinDB : result.decibel, MinDB, 0, 0, 100);
    }

    void FrequencyAnalyzer::calculateDominantFrequency(AnalyzerResult& result) 
    {
        auto fft = ArduinoFFT<float>(result.fftReal, result.fftImg, FFTDataSize, result.sampleRate);
        fft.windowing(FFTWindow::Hamming, FFTDirection::Forward);
        fft.compute(FFTDirection::Forward);
        fft.complexToMagnitude();

        // only check buckets between our min and max frequency
        uint8_t minFreqIndex = (uint8_t)((float)MinFrequency / ((float)result.sampleRate / (float)FFTDataSize));
        uint8_t maxFreqIndex = (uint8_t)((float)MaxFrequency / ((float)result.sampleRate / (float)FFTDataSize));

        unsigned short dominantFrequency = 0;
        short dominantFrequencyData = 0;

        for(unsigned short i = minFreqIndex; i < maxFreqIndex; i++)
        {
            if(result.fftReal[i] > dominantFrequencyData)
            {
                dominantFrequency = ((i * 1.0 * result.sampleRate) / FFTDataSize);
                dominantFrequencyData = result.fftReal[i];
            }
        }
        result.dominantFrequency = dominantFrequency > MaxFrequency ? MaxFrequency : dominantFrequency;
    }
}