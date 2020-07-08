#include <iostream>
#include <inttypes.h>
#include <client_commands.hpp>
#include <server_commands.hpp>

#include "websocket.hpp"

namespace Murli
{
    namespace Desktop
    {
        Websocket::Websocket(const std::string url) : _url(url)
        {
            ix::initNetSystem();

            _socket.setUrl(url);
            _socket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) { onMessage(msg); });
        }

        void Websocket::start()
        {
            _socket.start();
        }

        void Websocket::send(Server::Command command)
        {
            size_t commandSize = sizeof(command);
            std::vector<uint8_t> serializedCommand;
            serializedCommand.resize(sizeof(command));
            memcpy(&serializedCommand[0], &command, commandSize);                                 

            _socket.sendBinary(std::string(serializedCommand.begin(), serializedCommand.end()));
        }

        void Websocket::onMessage(const ix::WebSocketMessagePtr& msg)
        {
            if(msg->type == ix::WebSocketMessageType::Open)
                for(auto event : connectionEvents.getEventHandlers()) event.second(true);
            else if(msg->type == ix::WebSocketMessageType::Close)
                for(auto event : connectionEvents.getEventHandlers()) event.second(false);
            else if(msg->type == ix::WebSocketMessageType::Message)
            {
                if(msg->binary)
                {
                    Murli::Client::Command receivedCommand;
                    memcpy(&receivedCommand, &msg->str.c_str()[0], msg->str.length());
                    for(auto event : commandEvents.getEventHandlers()) event.second(receivedCommand);
                }
                else send({ 0, Server::MOD_DISTRIBUTED });
            }
        }
    }
}