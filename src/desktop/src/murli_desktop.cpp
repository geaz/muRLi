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
                [this](uint16_t dominantFrequency) { });
        }

        void MurliDesktop::run()
        {
            _websocket.start();
            while(true) { }
        }

        void MurliDesktop::onConnection(bool connected)
        {
            if(connected) _freqAnalyzer.start();
            else _freqAnalyzer.stop();
        }

        void MurliDesktop::onCommandReceived(Client::Command command)
        {
            switch(command.commandType)
            {
                case Murli::Client::CommandType::MESH_COUNT_REQUEST:
                    _websocket.send(createAnswer(command, Murli::Server::CommandType::MESH_COUNTED));
                    break;
                case Murli::Client::CommandType::MESH_UPDATE:
                    _websocket.send(createAnswer(command, Murli::Server::CommandType::MESH_UPDATED));
                    break;
                case Client::CommandType::SOURCE_UPDATE:
                    _currentSource = command.sourceCommand.source;
                    break;
                case Client::CommandType::MOD_REMOVED:
                    std::cout << "MOD REMOVED\n";
                    break;
                case Client::CommandType::ANALYZER_UPDATE:
                    std::cout << "Update: " << command.analyzerCommand.frequency << "\n";
                    break;
            }
        }

        void MurliDesktop::onFrequencyCalculated(uint16_t dominantFrequency)
        {
            if(_currentSource != Client::AnalyzerSource::Desktop) return;

            Server::Command serverCommand = { 0, Server::CommandType::EXTERNAL_ANALYZER };
            //Server::ExternalAnalyzerCommand analyzerCommand = { }

            /*
            float decibel;
            uint8_t volume;
            uint16_t frequency;
            std::array<uint8_t, 17> buckets;
            */ 
        }

        Server::Command MurliDesktop::createAnswer(Client::Command command, Server::CommandType answerCommandType)
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