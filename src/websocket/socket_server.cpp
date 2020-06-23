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

    void SocketServer::broadcast(MurliCommand command)
    {
        if(command.command == Murli::MESH_COUNT_REQUEST || command.command == Murli::MESH_UPDATE)
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

    void SocketServer::onCommandReceived(MeshCommandEvent event)
    {
        _meshCommandEvent = event;
    }

    void SocketServer::onMeshConnection(MeshConnectionEvent event)
    {
        _meshConnectionEvent = event;
    }
    
    void SocketServer::onModDistributed(MeshModDistributedEvent event)
    {
        _meshModDistributedEvent = event;
    }

    uint32_t SocketServer::connectedClients()
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
                if(_meshConnectionEvent != nullptr) _meshConnectionEvent();
                break;
            case WStype_CONNECTED:        
                Serial.println("Connection from: " + remoteIp.toString());
                if(_meshConnectionEvent != nullptr) _meshConnectionEvent();
                break;
            case WStype_BIN:
                Serial.println("Server receiving command ...");

                MurliCommand receivedCommand;
                memcpy(&receivedCommand, payload, length);

                if(receivedCommand.command == MESH_COUNTED 
                || receivedCommand.command == MESH_UPDATED
                || receivedCommand.command == MOD_DISTRIBUTED)
                {
                    if(_requestCommand.id == receivedCommand.id
                    || _modDistribution)
                    {
                        _answers++;
                        if(_answers == 1 || _receivedCommand.meshLedCount < receivedCommand.meshLedCount)
                        {
                            _receivedCommand = receivedCommand;
                        }
                        callFuncs = _answers == _webSocket.connectedClients();
                        if(callFuncs) receivedCommand = _receivedCommand;
                    }
                }
                else callFuncs = true;

                if(callFuncs)
                {
                    _requestCommand.id = 0;
                    _modDistribution = false;
                    if(receivedCommand.command == Murli::MOD_DISTRIBUTED && _meshModDistributedEvent != nullptr)
                        _meshModDistributedEvent(receivedCommand);
                    else if(receivedCommand.command != Murli::MOD_DISTRIBUTED && _meshCommandEvent != nullptr)
                        _meshCommandEvent(receivedCommand);
                }
                break;
            default:
                break;
        }
    }
}