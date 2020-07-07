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
                void onConnection(bool connected);
                void onCommandReceived(Client::Command command);
                void onFrequencyCalculated(uint16_t dominantFrequency);
                Server::Command createAnswer(Client::Command command, Server::CommandType answerCommandType);

                Websocket _websocket;
                FrequencyAnalyzer _freqAnalyzer;
                Client::AnalyzerSource _currentSource = Client::AnalyzerSource::Microphone;
        };
    }
}

#endif // MURLIDESKTOP_H