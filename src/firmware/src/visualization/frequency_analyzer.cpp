#include <arduinoFFT.h>
#include "frequency_include.hpp"
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
            result.fftReal[i] = analogRead(A0);
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
            sample = butterBandPass(result.fftReal[i]-512);
            result.fftReal[i] = sample;
            signalRMS += sample * sample;
        }

        float signalRMSflt = sqrt(signalRMS / FFTDataSize);
        result.decibel = 20.0*log10(signalRMSflt / 512);
        result.volume = Murli::map(result.decibel < MinDB ? MinDB : result.decibel, MinDB, 0, 0, 100);
    }

    void FrequencyAnalyzer::calculateDominantFrequency(AnalyzerResult& result) 
    {
        auto fft = ArduinoFFT<float>(result.fftReal, result.fftImg, FFTDataSize, SampleRate);
        fft.windowing(FFTWindow::Hamming, FFTDirection::Forward);
        fft.compute(FFTDirection::Forward);
        fft.complexToMagnitude();

        // Exponential Smoothing for the frequencies
        // To flatten frequency peaks
        float dominantFrequency = EfAlpha * fft.majorPeak() + (1.0f - EfAlpha) * _lastDominantFrequency;
        _lastDominantFrequency = dominantFrequency;

        result.dominantFrequency = dominantFrequency > MaxFrequency ? MaxFrequency : dominantFrequency;
    }

    // Band Pass Filter for <=100 >= 3500
    // http://www.schwietering.com/jayduino/filtuino/index.php
    float FrequencyAnalyzer::butterBandPass(const float value)
    {
        _bandPassValues[0] = _bandPassValues[1];
        _bandPassValues[1] = _bandPassValues[2];
		_bandPassValues[2] = _bandPassValues[3];
		_bandPassValues[3] = _bandPassValues[4];
		_bandPassValues[4] = (4.146985469705365968e-1 * value)
		    + (-0.20667198516656454244 * _bandPassValues[0])
			+ (-0.02794930525906139107 * _bandPassValues[1])
			+ (-0.18114773810038770074 * _bandPassValues[2])
			+ (1.40897920655145059143 * _bandPassValues[3]);
        return (_bandPassValues[0] + _bandPassValues[4]) - 2 * _bandPassValues[2];
    }
}