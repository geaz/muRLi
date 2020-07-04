#ifndef FREQUENCYANALYZER_H
#define FREQUENCYANALYZER_H

#include <ctime>
#include <chrono>
#include <RtAudio.h>

namespace Murli
{
    namespace Desktop
    {
        class FrequencyAnalyzer
        {
            public:
                void run();

            private:
                static int streamCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, 
                    double streamTime, RtAudioStreamStatus status, void *userData);

                std::chrono::high_resolution_clock::time_point _lastRun;
                RtAudio _adc = RtAudio(RtAudio::Api::WINDOWS_WASAPI);
                RtAudio::StreamParameters _parameters;
        };
    }
}

#endif // FREQUENCYANALYZER_H