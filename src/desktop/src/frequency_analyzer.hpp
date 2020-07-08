#ifndef FREQUENCYANALYZER_H
#define FREQUENCYANALYZER_H

#include <array>
#include <functional>
#include <RtAudio.h>
#include <kiss_fft.h>
#include <event_registration.hpp>

namespace Murli
{
    namespace Desktop
    {
        typedef std::function<void(float, uint8_t, uint16_t, std::array<uint8_t, BAR_COUNT>)> FrequencyEvent;

        static const uint16_t SampleRate = 9000;
        static const unsigned int FFTDataSize = 512;
        static const unsigned int HalfFFTDataSize = FFTDataSize / 2;

        static const float EfAlpha = 0.4f;
        static const int8_t MinDB = -50;

        class FrequencyAnalyzer
        {
            public:
                bool start();
                void stop();

                EventRegistration<FrequencyEvent> frequencyEvents;

            private:
                std::array<float, HalfFFTDataSize> calculateAmplitudes(const kiss_fft_cpx* cx) const;
                uint16_t getDominantFrequency(std::array<float, HalfFFTDataSize> amplitudes);

                static int streamCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, 
                    double streamTime, RtAudioStreamStatus status, void *userData);

                RtAudio _adc = RtAudio(RtAudio::Api::WINDOWS_WASAPI);
                RtAudio::StreamParameters _parameters;
                float _lastDominantFrequency = 0;
        };
    }
}

#endif // FREQUENCYANALYZER_H