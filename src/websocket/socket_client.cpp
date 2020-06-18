#include "socket_client.hpp"
#include "../led/patterns/led_blink_pattern.cpp"

namespace Murli
{
    SocketClient* SocketClientPointer = nullptr;

    SocketClient::SocketClient(LED& led) : _led(led)
    {
        Murli::SocketClientPointer = this;
    }

    void SocketClient::start(String socketIp)
    {        
        auto webSocketEvent = [](WStype_t type, uint8_t* payload, size_t length) 
        {
            std::shared_ptr<LedBlinkPattern> blinkPattern = nullptr;
            switch(type) 
            {
                case WStype_DISCONNECTED:              
                    Serial.println("Disconnected from WebSocket!");  
                    Murli::SocketClientPointer->_isConnected = false;
                    blinkPattern = std::make_shared<LedBlinkPattern>(Murli::Red, 3);
                    break;
                case WStype_CONNECTED:            
                    Serial.println("Connected to WebSocket!");
                    Murli::SocketClientPointer->_isConnected = true;                    
                    blinkPattern = std::make_shared<LedBlinkPattern>(Murli::Green, 3);
                    break;
                case WStype_BIN:
                    memcpy(&Murli::SocketClientPointer->_newReceivedCommand, payload, length);
                    Murli::SocketClientPointer->_hasNewCommand = true;
                    break;
                default:
                    // Not interested in other cases
                    break;
            }
            Murli::SocketClientPointer->_led.setPattern(std::move(blinkPattern));
        };

        Serial.println("Connecting to socket '" + socketIp + "' ...");
        _webSocket.begin(socketIp, 81, "/");
        _webSocket.onEvent(webSocketEvent);
        _webSocket.setReconnectInterval(5000);
    }

    void SocketClient::loop()
    {
        _webSocket.loop();
    }
    
    bool SocketClient::isConnected() const
    {
        return _isConnected;
    }

    bool SocketClient::hasNewCommand() const
    {
        return _hasNewCommand;
    }
    
    MurliCommand SocketClient::getNewCommand()
    {
        _hasNewCommand = false;
        return _newReceivedCommand;
    }
}