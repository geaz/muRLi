#include <iostream>
#include "murli_desktop.hpp"

namespace Murli
{
    namespace Desktop
    {
        MurliDesktop::MurliDesktop(std::string url) : _websocket(url)
        { 
            _websocket.commandEvents.addEventHandler(
                [this](Murli::Client::Command command) { onCommandReceived(command); });
            _websocket.connectionEvents.addEventHandler(
                [this](bool connected) { onConnection(connected); });
            _freqAnalyzer.frequencyEvents.addEventHandler(
                [this](const float decibel, const uint8_t volume, uint16_t dominantFrequency, std::array<uint8_t, BAR_COUNT> buckets) 
                { onFrequencyCalculated(decibel, volume, dominantFrequency, buckets); });
        }

        void MurliDesktop::run()
        {
            _websocket.start();
            while(true) { }
        }

        void MurliDesktop::onConnection(const bool connected)
        {
            if(connected) _freqAnalyzer.start();
            else _freqAnalyzer.stop();
        }

        void MurliDesktop::onCommandReceived(const Client::Command command)
        {
            switch(command.commandType)
            {
                case Murli::Client::CommandType::MESH_COUNT_REQUEST:
                    std::cout << "MESH COUNT REQUEST\n";
                    _websocket.send(createAnswer(command, Murli::Server::CommandType::MESH_COUNTED));
                    break;
                case Murli::Client::CommandType::MESH_UPDATE:
                    std::cout << "MESH UPDATE\n";
                    _websocket.send(createAnswer(command, Murli::Server::CommandType::MESH_UPDATED));
                    break;
                case Client::CommandType::SOURCE_UPDATE:
                    std::cout << "SOURCE UPDATE\n";
                    _currentSource = command.sourceCommand.source;
                    break;
                case Client::CommandType::MOD_REMOVED:
                    std::cout << "MOD REMOVED\n";
                    break;
                case Client::CommandType::ANALYZER_UPDATE:
                    break;
            }
        }

        void MurliDesktop::onFrequencyCalculated(const float decibel, const uint8_t volume, const uint16_t dominantFrequency, const std::array<uint8_t, BAR_COUNT> buckets)
        {
            if(_currentSource != Client::AnalyzerSource::Desktop) return;
            std::cout << dominantFrequency << "\n";

            Server::Command serverCommand = { 0, Server::CommandType::EXTERNAL_ANALYZER };
            Server::ExternalAnalyzerCommand analyzerCommand = { decibel, volume, dominantFrequency, buckets };
            serverCommand.externalAnalyzerCommand = analyzerCommand;

            _websocket.send(serverCommand); 
        }

        Server::Command MurliDesktop::createAnswer(const Client::Command command, const Server::CommandType answerCommandType) const
        {
            Server::Command answerCommand = { command.id, answerCommandType };
            if(answerCommandType == Murli::Server::MESH_COUNTED)
            {
                Server::CountedCommand counted = { command.countCommand.meshLedCount };
                answerCommand.countedCommand = counted;
            }
            return answerCommand;
        }
    }
}