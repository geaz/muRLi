#ifndef MURLIDESKTOP_H
#define MURLIDESKTOP_H

#include <client_commands.hpp>
#include "websocket.hpp"
#include "frequency_analyzer.hpp"

namespace Murli
{
    namespace Desktop
    {
        class MurliDesktop
        {
            public:
                MurliDesktop(std::string url);

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

#endif // MURLIDESKTOP_H