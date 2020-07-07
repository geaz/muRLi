#ifndef FREQUENCYANALYZER_H
#define FREQUENCYANALYZER_H

#include <functional>
#include <RtAudio.h>
#include <kiss_fft.h>
#include <event_registration.hpp>

namespace Murli
{
    namespace Desktop
    {
        typedef std::function<void(uint16_t)> FrequencyEvent;

        static const uint16_t SampleRate = 9000;
        static const unsigned int FFTDataSize = 512;
        static const unsigned int HalfFFTDataSize = FFTDataSize / 2;

        class FrequencyAnalyzer
        {
            public:
                bool start();
                bool stop();

                EventRegistration<FrequencyEvent> frequencyEvents;

            private:
                uint16_t getDominantFrequency(const kiss_fft_cpx* cx) const;
                static int streamCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, 
                    double streamTime, RtAudioStreamStatus status, void *userData);

                RtAudio _adc = RtAudio(RtAudio::Api::WINDOWS_WASAPI);
                RtAudio::StreamParameters _parameters;
        };
    }
}

#endif // FREQUENCYANALYZER_H