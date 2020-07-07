#include <cmath>
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

                    _parameters.deviceId = _adc.getDefaultOutputDevice();
                    _parameters.nChannels = 2;
                    _parameters.firstChannel = 0;
                    
                    _adc.openStream(nullptr, &_parameters, RTAUDIO_SINT16, SampleRate, &bufferFrames, 
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

        uint16_t FrequencyAnalyzer::getDominantFrequency(const kiss_fft_cpx* cx) const
        {
            uint16_t highestBin = 0;
            float highestAmplitude = 0;
            float freqData[HalfFFTDataSize];

            for(uint16_t i = 0; i < HalfFFTDataSize; i++)
            {
                freqData[i] = sqrt(pow(cx[i].r, 2) + pow(cx[i].i, 2));
                if(freqData[i] > highestAmplitude)
                {
                    highestAmplitude = freqData[i];
                    highestBin = i;
                }
            }
            return (uint16_t)(highestBin * ((float)9000 / (float)256));
        }

        /* static */ int FrequencyAnalyzer::streamCallback(void *outputBuffer, void *inputBuffer, 
            unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData)
        {            
            FrequencyAnalyzer* freqAnalyzer = (FrequencyAnalyzer*)userData;

            kiss_fft_cpx cx_in[FFTDataSize], cx_out[FFTDataSize];       
            kiss_fft_cfg cfg = kiss_fft_alloc(FFTDataSize, false, nullptr, nullptr);
            for(uint16_t i = 0; i < FFTDataSize; i++)
            {
                cx_in[i].r = (float)((int16_t *)inputBuffer)[i];
                cx_in[i].i = 0;
            }
            kiss_fft(cfg , cx_in, cx_out);
            uint16_t dominantFrequency = freqAnalyzer->getDominantFrequency(&cx_out[0]);
            kiss_fft_free(cfg);

            for(auto event : freqAnalyzer->frequencyEvents.getEventHandlers()) event.second(dominantFrequency);

            return 0;
        }
    }
}