#include "frequency_analyzer.hpp"
#include <iostream>
#include <RtAudio.h>

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
            
            unsigned int sampleRate = 44100;
            unsigned int bufferFrames = 512;

            auto info = _adc.getDeviceInfo(_parameters.deviceId);
            std::cout << info.nativeFormats << "\n";

            try
            {
                _adc.openStream(nullptr, &_parameters, RTAUDIO_FLOAT32, sampleRate, &bufferFrames, 
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
            FrequencyAnalyzer* freqAnalyzer = (FrequencyAnalyzer*)userData;

            if ( status ) std::cout << "Stream over/underflow detected." << std::endl;
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = now - freqAnalyzer->_lastRun;
            std::cout << duration.count() << "\n";            
            freqAnalyzer->_lastRun = now;

            //std::cout << outputBuffer << ' ' << ((float *)outputBuffer)[0] << ' ' << inputBuffer << ' ' << ((float *)inputBuffer)[0] << ' ' << nBufferFrames << std::endl;
            return 0;
        }
    }
}