#include <arduinoFFT.h>
#include "../shared.hpp"
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
        calculateVolumeAndBandPass(result);
        if(result.volume > 0) calculateDominantFrequency(result);
        else result.dominantFrequency = 0;
        
        return result;
    }

    void FrequencyAnalyzer::collectSamples(AnalyzerResult& result)
    {
        unsigned long startMicros = micros();
        for (int i = 0; i < FFTDataSize; i++)
        {
            result.fftReal[i] = (analogRead(A0) - 512.0f) / 512.0f;
            result.fftImg[i] = 0;

            while(micros() - startMicros < SamplePerioduSec){
                //empty loop to wait
            }
            startMicros += SamplePerioduSec;
        }
    }

    void FrequencyAnalyzer::calculateVolumeAndBandPass(AnalyzerResult& result)
    {
        float signalRMS = 0;
        float sample = 0;
        for (int i = 0; i < FFTDataSize; i++)
        {
            // Using Bandpass Filter here, because muRLi 
            // samples include noise in lower frequencies.
            // Filtering them away with help of the Band-Pass-Filter
            // muRLi will not be able to recognize <= 100hz frequencies
            // very well...
            sample = _bandPassFilter.filter(result.fftReal[i]);
            result.fftReal[i] = sample;
            signalRMS += sample * sample;
        }

        float signalRMSflt = sqrt(signalRMS / FFTDataSize);
        result.decibel = 20.0*log10(signalRMSflt);
        result.volume = Murli::mapf(result.decibel < MinDB ? MinDB : result.decibel, MinDB, 0, 0, 100);
    }

    void FrequencyAnalyzer::calculateDominantFrequency(AnalyzerResult& result) 
    {
        auto fft = ArduinoFFT<float>(result.fftReal, result.fftImg, FFTDataSize, SampleRate);
        fft.windowing(FFTWindow::Hamming, FFTDirection::Forward);
        fft.compute(FFTDirection::Forward);
        fft.complexToMagnitude();

        uint16_t highestBin = 0;
        float highestAmplitude = 0;
        for(uint16_t i = 0; i < MaxFreqBinIndex; i++)
        {
            if(result.fftReal[i] > highestAmplitude)
            {
                highestAmplitude = result.fftReal[i];
                highestBin = i;
            }
        }

        result.dominantFrequency = highestBin * ((float)SampleRate / (float)FFTDataSize);        
        // Exponential Smoothing for the frequencies
        // To flatten frequency peaks
        result.dominantFrequency = EfAlpha * result.dominantFrequency + (1.0f - EfAlpha) * _lastDominantFrequency;
        _lastDominantFrequency = result.dominantFrequency;
    }
}