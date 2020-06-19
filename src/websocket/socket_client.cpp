#include "socket_client.hpp"
#include "../led/patterns/led_blink_pattern.cpp"

namespace Murli
{
    SocketClient::SocketClient(LED& led) : _led(led) { }

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

    void SocketClient::sendCommand(MurliCommand command)
    {
        size_t commandSize = sizeof(command);
        uint8_t serializedCommand[commandSize];
        memcpy(&serializedCommand, &command, commandSize);

        _webSocket.sendBIN(&serializedCommand[0], commandSize);
    }

    void SocketClient::onModReceived(MeshModEvent event)
    {
        _meshModEvent = event;
    }

    void SocketClient::onCommandReceived(MeshCommandEvent event)
    {
        _meshCommandEvent = event;
    }
    
    bool SocketClient::isConnected() const
    {
        return _isConnected;
    }

    void SocketClient::clientEvent(WStype_t type, uint8_t* payload, size_t length)
    {
        std::shared_ptr<LedBlinkPattern> blinkPattern = nullptr;
        switch(type)
        {
            case WStype_DISCONNECTED:              
                Serial.println("Disconnected from WebSocket!");  
                _isConnected = false;
                blinkPattern = std::make_shared<LedBlinkPattern>(Murli::Red, 3);
                break;
            case WStype_CONNECTED:            
                Serial.println("Connected to WebSocket!");
                _isConnected = true;                    
                blinkPattern = std::make_shared<LedBlinkPattern>(Murli::Green, 3);
                break;
            case WStype_BIN:
                MurliCommand receivedCommand;
                memcpy(&receivedCommand, payload, length);
                if(_meshCommandEvent != nullptr) _meshCommandEvent(receivedCommand);
                break;
            case WStype_TEXT:
                if(_meshModEvent != nullptr) _meshModEvent(std::string((char*)payload));
                break;
            default:
                // Not interested in other cases
                break;
        }
        _led.setPattern(std::move(blinkPattern));
    }
}