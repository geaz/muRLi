#include "socket_client.hpp"

namespace Murli
{
    SocketClient* SocketClientPointer = NULL;

    SocketClient::SocketClient(LED& led) : _led(led)
    {
        Murli::SocketClientPointer = this;
    }

    void SocketClient::start(String socketIp)
    {        
        auto webSocketEvent = [](WStype_t type, uint8_t* payload, size_t length) 
        {
            switch(type) 
            {
                case WStype_DISCONNECTED:              
                    Serial.println("Disconnected from WebSocket!");  
                    Murli::SocketClientPointer->_isConnected = false;
                    Murli::SocketClientPointer->_led.blink(Murli::Red, 3);
                    break;
                case WStype_CONNECTED:            
                    Serial.println("Connected to WebSocket!");
                    Murli::SocketClientPointer->_isConnected = true;
                    Murli::SocketClientPointer->_led.blink(Murli::Green, 3);
                    break;
                case WStype_BIN:
                    memcpy(&Murli::SocketClientPointer->_newReceivedCommand, payload, length);
                    Murli::SocketClientPointer->_hasNewCommand = true;
                    break;
                default:
                    // Not interested in other cases
                    break;
            }
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