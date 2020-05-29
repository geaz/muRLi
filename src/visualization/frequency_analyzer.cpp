#include "Adafruit_ZeroFFT.h"
#include "frequency_analyzer.hpp"

namespace Murli
{
    /**
     * @brief   On each call this method collects samples and analyzes the volume and frequency.
     *          The results are saved into private members, accessable by public getters. 
     */
    void FrequencyAnalyzer::loop()
    {
        double sampleRMS = collectSamples();        
        _volume = calculateVolume(sampleRMS);            
        _dominantFrequency = calculateDominantFrequency();

        if(sampleRMS > lastRMSDeque.front())
        {
            Serial.print("Sample RMS: " + String(sampleRMS));
            Serial.print("| Last Sample RMS: " + String(lastRMSDeque.front()));
            Serial.print("| Volume: " + String(_volume));
            Serial.print("| Dominant Frequency: " + String(_dominantFrequency) + " Hz");
            Serial.println("");
        }        
        lastRMSDeque.push_front(sampleRMS);
        if(lastRMSDeque.size() > MaxRMSCount) lastRMSDeque.pop_back();
    }    

    /**
     * @brief This method returns a consollidated reprensentation of the calculated frequency buckets.
     * 
     * @param buckets Define into how many buckets the data should be combined
     * @param maxValue Define the new max value for each bucket. The values will be mapped into this new range (0-maxValue)
     * @return std::vector<uint8_t> Returns a vector with the mapped value of each bucket
     */
    std::vector<uint8_t> FrequencyAnalyzer::getFrequencyRange(uint8_t buckets, uint8_t maxValue)
    {
        std::vector<uint8_t> combinedBuckets;        
        
        // only check buckets between our min and max frequency
        uint8_t minFreqIndex = getUpperBoundFreqBucketIndex(MinFrequency);
        uint8_t maxFreqIndex = getUpperBoundFreqBucketIndex(MaxFrequency);

        int16_t originalMaxValue = 0;
        for(uint8_t i = minFreqIndex; i < maxFreqIndex; i++)
        {
            originalMaxValue = max(originalMaxValue, fftData[i]);
        }
        
        uint8_t splitSize = (maxFreqIndex - minFreqIndex + 1) / buckets;
        for(uint8_t i = 0; i < buckets; i++)
        {
            uint32_t combinedValue = 0;
            for(uint8_t j = 0; j < splitSize; j++)
            {
                combinedValue += fftData[j + (i * splitSize) + minFreqIndex /* offset */];
            }
            
            // map the current volume to the value range of the current frequency
            // this way the bars will raise dependending on the volume
            // then map it to the new max value
            uint8_t newMaxValue =  map(getVolume(), 0, 100, 0, combinedValue/splitSize);
            newMaxValue = map(newMaxValue, 0, originalMaxValue, 0, maxValue);

            combinedBuckets.push_back(newMaxValue);
        }
        return combinedBuckets;
    }

    uint8_t FrequencyAnalyzer::getVolume() { return _volume; }
    uint16_t FrequencyAnalyzer::getDominantFrequency() { return _dominantFrequency; }

    double FrequencyAnalyzer::collectSamples()
    {
        unsigned short signalMax = 0;
        unsigned short signalMin = 1024;
        unsigned short signalCount = 0;
        unsigned long signalSquare = 0;
        unsigned long startMillis = millis();    

        while(millis() - startMillis < SampleWindow)
        {
            unsigned short sample = analogRead(A0);
            signalMin = min(signalMin, sample);
            signalMax = max(signalMax, sample);
            signalSquare += sample * sample;
            
            if(signalCount < FFTDataSize) fftData[signalCount] = sample;
            signalCount++;
        }

        double sampleRMS = sqrt(signalSquare / signalCount);
        sampleRMS = abs(sampleRMS - 512); 
        sampleRMS = sampleRMS <= Noise ? 0 : sampleRMS - Noise;

        return sampleRMS;
    }

    uint8_t FrequencyAnalyzer::calculateVolume(const double sampleRMS) const
    {
        double minRMS = sampleRMS;
        double maxRMS = sampleRMS;
        for(double lastRMS : lastRMSDeque) {
            minRMS = min(minRMS, lastRMS);
            maxRMS = max(maxRMS, lastRMS);
        }
        maxRMS = max(maxRMS, MinimumMaxRMS);

        return map(sampleRMS, minRMS, maxRMS, 0, 100);
    }

    uint16_t FrequencyAnalyzer::calculateDominantFrequency() 
    {
        ZeroFFT(fftData, FFTDataSize);
        unsigned short dominantFrequency = 0;
        short dominantFrequencyData = 0;

        // only check buckets between our min and max frequency
        uint8_t minFreqIndex = getUpperBoundFreqBucketIndex(MinFrequency);
        uint8_t maxFreqIndex = getUpperBoundFreqBucketIndex(MaxFrequency);

        // data is only meaningful up to sample rate/2, discard the other half
        for(unsigned short i = minFreqIndex; i < maxFreqIndex; i++)
        {
            if(fftData[i] > dominantFrequencyData)
            {
                dominantFrequency = FFT_BIN(i, SampleRate, FFTDataSize);
                dominantFrequencyData = fftData[i];
            }
        }
        return dominantFrequency > MaxFrequency ? MaxFrequency : dominantFrequency;
    }

    uint8_t FrequencyAnalyzer::getUpperBoundFreqBucketIndex(uint16_t frequency)
    {
        uint8_t bucketIndex = 0;
        for(uint8_t i = 0; i < FFTDataSize / 2; i++)
        {
            if(FFT_BIN(i, SampleRate, FFTDataSize) >= frequency)
            {
                bucketIndex = i;
                break;
            }
        }
        return bucketIndex;
    }
}