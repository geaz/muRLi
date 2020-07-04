#include "frequency_analyzer.hpp"
#include <iostream>
#include <RtAudio.h>
#include <kiss_fft.h>
#include <cmath>

namespace Murli
{
    namespace Desktop
    {
        void FrequencyAnalyzer::run()
        {
            if(_adc.getDeviceCount() < 1) return;

            _parameters.deviceId = _adc.getDefaultOutputDevice();
            _parameters.nChannels = 2;
            _parameters.firstChannel = 0;
            
            unsigned int sampleRate = 9000;
            unsigned int bufferFrames = 512;

            auto info = _adc.getDeviceInfo(_parameters.deviceId);
            std::cout << info.nativeFormats << "\n";

            try
            {
                _adc.openStream(nullptr, &_parameters, RTAUDIO_SINT16, sampleRate, &bufferFrames, 
                    &FrequencyAnalyzer::streamCallback, (void *)this);
                _adc.startStream();
            }
            catch (RtAudioError& e)
            {
                e.printMessage();
            }
        }

        int FrequencyAnalyzer::streamCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, 
                    double streamTime, RtAudioStreamStatus status, void *userData)
        {
            kiss_fft_cpx cx_in[512], cx_out[512];       
            kiss_fft_cfg cfg = kiss_fft_alloc(512, false, nullptr, nullptr);
            for(uint16_t i = 0; i < nBufferFrames; i++)
            {
                cx_in[i].r = (float)((int16_t *)inputBuffer)[i];
                cx_in[i].i = 0;
            }
            kiss_fft(cfg , cx_in, cx_out);
            float freqData[512/2];
            uint16_t highestBin = 0;
            float highestAmplitude = 0;
            for(uint16_t i = 0; i < nBufferFrames/2; i++)
            {
                freqData[i] = sqrt(pow(cx_out[i].r, 2) + pow(cx_out[i].i, 2));
                if(freqData[i] > highestAmplitude)
                {
                    highestAmplitude = freqData[i];
                    highestBin = i;
                }
            }
            std::cout << "Dominant Frequency: " << highestBin * ((float)9000 / (float)256) << "\n";
            kiss_fft_free(cfg);

            FrequencyAnalyzer* freqAnalyzer = (FrequencyAnalyzer*)userData;

            if ( status ) std::cout << "Stream over/underflow detected." << std::endl;
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = now - freqAnalyzer->_lastRun;
            std::cout << duration.count() << "\n";            
            freqAnalyzer->_lastRun = now;

            std::cout << outputBuffer << ' ' << ((float *)outputBuffer)[0] << ' ' << inputBuffer << ' ' << ((float *)inputBuffer)[0] << ' ' << nBufferFrames << std::endl;
            return 0;
        }
    }
}