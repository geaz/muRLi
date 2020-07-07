#include "socket_client.hpp"

namespace Murli
{
    void SocketClient::start(String socketIp)
    { 
        Serial.println("Connecting to socket '" + socketIp + "' ...");
        _webSocket.begin(socketIp, 81, "/");
        _webSocket.onEvent([this](WStype_t type, uint8_t* payload, size_t length){ clientEvent(type, payload, length); });
        _webSocket.setReconnectInterval(5000);
    }

    void SocketClient::loop()
    {
        _webSocket.loop();
    }

    void SocketClient::sendCommand(Server::Command command)
    {
        size_t commandSize = sizeof(command);
        uint8_t serializedCommand[commandSize];
        memcpy(&serializedCommand, &command, commandSize);

        _webSocket.sendBIN(&serializedCommand[0], commandSize);
    }
    
    bool SocketClient::isConnected() const
    {
        return _isConnected;
    }

    void SocketClient::clientEvent(WStype_t type, uint8_t* payload, size_t length)
    { 
        switch(type)
        {
            case WStype_DISCONNECTED:              
                Serial.println("Disconnected from WebSocket!");  
                _isConnected = false;
                break;
            case WStype_CONNECTED:            
                Serial.println("Connected to WebSocket!");
                _isConnected = true;
                break;
            case WStype_BIN:
                Client::Command receivedCommand;
                memcpy(&receivedCommand, payload, length);
                for(auto event : clientCommandEvents.getEventHandlers()) event.second(receivedCommand);
                break;
            case WStype_TEXT:
                for(auto event : meshModEvents.getEventHandlers()) event.second(std::string((char*)payload));
                break;
            default:
                break;
        }
    }
}