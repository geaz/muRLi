#include <cmath>
#include <frequency_include.hpp>
#include "frequency_analyzer.hpp"

namespace Murli
{
    namespace Desktop
    {
        bool FrequencyAnalyzer::start()
        {
            bool started = false;
            if(_adc.getDeviceCount() < 1) return started;

            try
            {
                if(!_adc.isStreamOpen())
                {                    
                    unsigned int bufferFrames = FFTDataSize;
                    uint32_t deviceId = _adc.getDefaultOutputDevice();
                    RtAudio::DeviceInfo deviceInfo = _adc.getDeviceInfo(deviceId);

                    _parameters.deviceId = _adc.getDefaultOutputDevice();
                    _parameters.nChannels = deviceInfo.outputChannels;
                    _parameters.firstChannel = 0;
                    
                    _adc.openStream(nullptr, &_parameters, RTAUDIO_FLOAT32, SampleRate, &bufferFrames, 
                        &FrequencyAnalyzer::streamCallback, (void *)this);
                }
                _adc.startStream();
                started = true;
            }
            catch (RtAudioError& e)
            {
                e.printMessage();
            }
            return started;
        }

        void FrequencyAnalyzer::stop()
        {
            if(_adc.isStreamRunning()) _adc.stopStream();
        }

        std::array<float, HalfFFTDataSize> FrequencyAnalyzer::calculateAmplitudes(const kiss_fft_cpx* cx) const
        {
            std::array<float, HalfFFTDataSize> amplitudes;
            for(uint16_t i = 0; i < HalfFFTDataSize; i++)
            {
                amplitudes[i] = sqrtf(powf(cx[i].r, 2) + powf(cx[i].i, 2));
            }
            return amplitudes;
        }

        uint16_t FrequencyAnalyzer::getDominantFrequency(std::array<float, HalfFFTDataSize> amplitudes)
        {
            uint16_t highestBin = 0;
            float highestAmplitude = 0;
            for(uint16_t i = 0; i < HalfFFTDataSize; i++)
            {
                if(amplitudes[i] > highestAmplitude)
                {
                    highestAmplitude = amplitudes[i];
                    highestBin = i;
                }
            }

            // Get the Frequency represented by the highest Bin
            float dominantFrequency = highestBin * ((float)SampleRate / (float)FFTDataSize);
            
            // Exponential Smoothing for the frequencies
            // To flatten frequency peaks
            dominantFrequency = EfAlpha * dominantFrequency + (1.0f - EfAlpha) * _lastDominantFrequency;  

            // Ensure Frequency between Max and Min
            dominantFrequency = dominantFrequency > Murli::MaxFrequency ? Murli::MaxFrequency : dominantFrequency;
            dominantFrequency = dominantFrequency < Murli::MinFrequency ? 0 : dominantFrequency;
            
            _lastDominantFrequency = dominantFrequency;
            return (uint16_t)dominantFrequency;
        }

        /* static */ int FrequencyAnalyzer::streamCallback(void *outputBuffer, void *inputBuffer, 
            unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData)
        {
            FrequencyAnalyzer* freqAnalyzer = (FrequencyAnalyzer*)userData;

            kiss_fft_cpx cx_in[FFTDataSize], cx_out[FFTDataSize];       
            kiss_fft_cfg cfg = kiss_fft_alloc(FFTDataSize, false, nullptr, nullptr);

            float signalRMS = 0;
            uint8_t channelCount = freqAnalyzer->_parameters.nChannels;
            for(uint32_t i = 0; i < FFTDataSize; i++)
            {
                // Sum the samples of each channel
                float sum = 0;
                for(uint8_t j = 0; j < channelCount; j++)
                {
                    sum += ((float *)inputBuffer)[(i * channelCount) + j];
                }

                float sample = sum / channelCount;
                signalRMS += sample * sample;
                cx_in[i].r = sample;
                cx_in[i].i = 0;
                cx_out[i].r = 0;
                cx_out[i].i = 0;
            }
            
            kiss_fft(cfg, cx_in, cx_out);
            kiss_fft_free(cfg);

            float signalRMSflt = sqrtf(signalRMS / FFTDataSize);
            float decibel = 20.0f * log10(signalRMSflt);
            uint8_t volume = (uint8_t) Murli::map(decibel < MinDB ? MinDB : decibel, MinDB, 0, 0, 100);

            std::array<float, HalfFFTDataSize> amplitudes = freqAnalyzer->calculateAmplitudes(&cx_out[0]);
            uint16_t dominantFrequency = freqAnalyzer->getDominantFrequency(amplitudes);
            std::array<uint8_t, BAR_COUNT> frequencyBars = Murli::getFrequencyBars(amplitudes.data(), Desktop::SampleRate, Desktop::FFTDataSize);

            for(auto event : freqAnalyzer->frequencyEvents.getEventHandlers())
                event.second(decibel, volume, dominantFrequency, frequencyBars);

            return 0;
        }
    }
}