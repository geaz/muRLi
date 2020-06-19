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
        size_t commandSize = sizeof(command);
        uint8_t serializedCommand[commandSize];
        memcpy(&serializedCommand, &command, commandSize);

        _webSocket.broadcastBIN(&serializedCommand[0], commandSize);
    }

    void SocketServer::onCommandReceived(MeshCommandEvent event)
    {
        _meshCommandEvent = event;
    }

    void SocketServer::onMeshConnection(MeshConnectionEvent event)
    {
        _meshConnectionEvent = event;
    }

    uint32_t SocketServer::connectedClients()
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
                if(_meshCommandEvent != nullptr) _meshCommandEvent(receivedCommand);
                break;
            default:
                // Not interested in other cases
                break;
        }
    }
}