#include <iostream>
#include "murli_desktop.hpp"

namespace Murli
{
    namespace Desktop
    {
        MurliDesktop::MurliDesktop(std::string url) : _websocket(url)
        { 
            _websocket.addOnCommandReceived([this](Murli::Client::Command command) { onCommandReceived(command); });
        }

        void MurliDesktop::run()
        {
            _websocket.start();
            while(true) { }
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
                    std::cout << "SOURCE: " << command.sourceCommand.source << "\n";
                    break;
                case Client::CommandType::ANALYZER_UPDATE:
                    std::cout << "Update: " << command.analyzerCommand.frequency << "\n";
                    break;
            }
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