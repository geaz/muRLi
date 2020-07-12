#ifndef MURLIAPI_H
#define MURLIAPI_H

#include <client_commands.hpp>
#include "websocket.hpp"
#include "frequency_analyzer.hpp"
#include <event_registration.hpp>

namespace Murli
{
    namespace Api
    {
        class MurliApi
        {
            public:
                MurliApi(std::string url, Client::AnalyzerSource source);
                void run();

            private:
                void onConnection(const bool connected);
                void onCommandReceived(const Client::Command command);
                void onFrequencyCalculated(const float decibel, const uint8_t volume, const uint16_t dominantFrequency, const std::array<uint8_t, BAR_COUNT> buckets);
                Server::Command createAnswer(const Client::Command command, const Server::CommandType answerCommandType) const;

                Websocket _websocket;
                FrequencyAnalyzer _freqAnalyzer;
                Client::AnalyzerSource _currentSource = Client::AnalyzerSource::Microphone;
        };
    }
}

extern "C" {
    typedef void* murli_api;

    __declspec(dllexport) murli_api murli_api_start(const char *url, int source)
    {
        return new Murli::Api::MurliApi(url, static_cast<Murli::Client::AnalyzerSource>(source));
    }

    __declspec(dllexport) void murli_api_on_freq(
        murli_api m,
        void (*fn)(murli_api m, uint8_t volume, float decibel, uint16_t dominant_frequency))
    {
       // static_cast<Murli::Api::MurliApi*>(m)->
    }
}

#endif // MURLIDESKTOP_H