#include "Adafruit_ZeroFFT.h"
#include "sound_visualization.hpp"

namespace Murli
{
    std::tuple<uint8_t, uint16_t> SoundVisualization::getVolAndFreq()
    {
        double sampleRMS = collectSamples();
        uint16_t dominantFrequency = getDominantFrequency();

        std::tuple<uint8_t, uint16_t> frameTuple(0, 0);
        if(sampleRMS > lastRMSDeque.front() && dominantFrequency >= MinFrequency)
        {
            unsigned char volumeBrightness = getVolumeBrightness(sampleRMS);
            frameTuple = std::tuple<uint8_t, uint16_t>(sampleRMS, dominantFrequency);

            Serial.print("Sample RMS: " + String(sampleRMS));
            Serial.print("| Last Sample RMS: " + String(lastRMSDeque.front()));
            Serial.print("| New Brightness: " + String(volumeBrightness));
            Serial.print("| Dominant Frequency: " + String(dominantFrequency) + " Hz");
            Serial.println("");
        }        
        lastRMSDeque.push_front(sampleRMS);
        if(lastRMSDeque.size() > MaxRMSCount) lastRMSDeque.pop_back();

        return frameTuple;
    }

    double SoundVisualization::collectSamples()
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

    uint8_t SoundVisualization::getVolumeBrightness(const double sampleRMS) const
    {
        double minRMS = sampleRMS;
        double maxRMS = sampleRMS;
        for(double lastRMS : lastRMSDeque) {
            minRMS = min(minRMS, lastRMS);
            maxRMS = max(maxRMS, lastRMS);
        }
        maxRMS = max(maxRMS, MinimumMaxRMS);

        return map(sampleRMS, minRMS, maxRMS, 0, 255);
    }

    uint16_t SoundVisualization::getDominantFrequency() 
    {
        ZeroFFT(fftData, FFTDataSize);
        unsigned short dominantFrequency = 0;
        short dominantFrequencyData = 0;

        // Discard first two bins (We want to ignore silence)
        for(unsigned short i = 2; i < FFTDataSize / 2; i++)
        {
            if(fftData[i] > dominantFrequencyData)
            {
                dominantFrequency = FFT_BIN(i, SampleRate, FFTDataSize);
                dominantFrequencyData = fftData[i];
            }
        }
        return dominantFrequency > MaxFrequency ? MaxFrequency : dominantFrequency;
    }
}