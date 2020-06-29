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
        if (command.commandType == Client::MESH_COUNT_REQUEST
        || command.commandType == Client::MESH_UPDATE)
        {
            _requestCommand = command;
        }

        size_t commandSize = sizeof(command);
        uint8_t serializedCommand[commandSize];
        memcpy(&serializedCommand, &command, commandSize);

        _webSocket.broadcastBIN(&serializedCommand[0], commandSize);
    }

    void SocketServer::broadcastMod(std::string& mod)
    {
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
        bool callFuncs = false;
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
                Serial.println("Server receiving command ...");

                Server::Command receivedCommand;
                memcpy(&receivedCommand, payload, length);
                if(allAnswerReceived(receivedCommand))
                {
                    _answers = 0;
                    _requestCommand.id = 0;
                    _modDistribution = false;
                    for(ServerCommandEvent event : _serverCommandEvents) event(_receivedCommand);
                }
                break;
            default:
                break;
        }
    }

    bool SocketServer::allAnswerReceived(Server::Command receivedCommand)
    {
        bool allReceived = false;

        // We want to wait for the answers of all connected clients
        // MESH_CONNECTION is not an answer command - thats why we exclude it here
        if(receivedCommand.commandType != Server::MESH_CONNECTION)
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