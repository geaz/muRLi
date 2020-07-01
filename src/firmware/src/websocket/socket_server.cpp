#include "socket_server.hpp"

namespace Murli
{
    SocketServer::SocketServer()
    {
        _webSocket.begin();
        _webSocket.onEvent([this](uint8_t num, WStype_t type, uint8_t * payload, size_t length){ serverEvent(num, type, payload, length); });
    }

    void SocketServer::loop()
    {
        _webSocket.loop();
    }

    void SocketServer::broadcast(Client::Command command)
    {
        // Only MESH_COUNT_REQUEST and MESH_UPDATE are awaiting answers
        if (command.commandType == Client::MESH_COUNT_REQUEST
        || command.commandType == Client::MESH_UPDATE)
        {
            _answers = 0;
            _requestCommand = command;
        }

        size_t commandSize = sizeof(command);
        uint8_t serializedCommand[commandSize];
        memcpy(&serializedCommand, &command, commandSize);

        _webSocket.broadcastBIN(&serializedCommand[0], commandSize);
    }

    void SocketServer::broadcastMod(std::string& mod)
    {
        _answers = 0;
        _modDistribution = true;
        _webSocket.broadcastTXT(mod.c_str(), mod.length());
    }

    void SocketServer::addOnMeshConnection(MeshConnectionEvent event)
    {
        _meshConnectionEvents.push_back(event);
    }

    void SocketServer::addOnCommandReceived(ServerCommandEvent event)
    {
        _serverCommandEvents.push_back(event);
    }

    uint32_t SocketServer::getClientsCount()
    {
        return _webSocket.connectedClients();
    }

    void SocketServer::serverEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
    {
        IPAddress remoteIp = _webSocket.remoteIP(num);
        switch(type) 
        {
            case WStype_DISCONNECTED:              
                Serial.println("Client disconnected!");
                for(MeshConnectionEvent event : _meshConnectionEvents) event();
                break;
            case WStype_CONNECTED:        
                Serial.println("Connection from: " + remoteIp.toString());
                for(MeshConnectionEvent event : _meshConnectionEvents) event();
                break;
            case WStype_BIN:
                Server::Command receivedCommand;
                memcpy(&receivedCommand, payload, length);
                if(allAnswerReceived(receivedCommand))
                {
                    _requestCommand.id = 0;
                    _modDistribution = false;
                    for(ServerCommandEvent event : _serverCommandEvents)
                    {
                        if(event)
                        {
                            Serial.println("jau");
                            event(_receivedCommand);
                        }
                        else Serial.println("nau");
                    }   
                }
                break;
        }
    }

    bool SocketServer::allAnswerReceived(Server::Command receivedCommand)
    {
        bool allReceived = false;
        // We want to wait for the answers of all connected clients
        // EXTERNAL_ANALYZER and MESH_CONNECTION are not answer commands - thats why we exclude them here
        if(receivedCommand.commandType != Server::EXTERNAL_ANALYZER
        && receivedCommand.commandType != Server::MESH_CONNECTION)
        {            
            if(_requestCommand.id == receivedCommand.id || _modDistribution)
            {
                _answers++;
                if (_answers == 1
                || (receivedCommand.commandType == Server::MESH_COUNTED 
                && _receivedCommand.countedCommand.meshLedCount < receivedCommand.countedCommand.meshLedCount))
                {
                    _receivedCommand = receivedCommand;
                }
                allReceived = _answers == _webSocket.connectedClients();
            }
        }
        else
        {
            allReceived = true;
            _receivedCommand = receivedCommand;
        }
        return allReceived;
    }
}