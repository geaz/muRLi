#include "socket_client.hpp"

namespace Murli
{
    SocketClient* SocketClientPointer = NULL;

    SocketClient::SocketClient(LED& led) : led(led)
    {
        Murli::SocketClientPointer = this;
    }

    void SocketClient::start(String socketIp)
    {        
        auto webSocketEvent = [](WStype_t type, uint8_t * payload, size_t length) 
        {
            switch(type) 
            {
                case WStype_DISCONNECTED:              
                    Serial.println("Disconnected from WebSocket!");  
                    Murli::SocketClientPointer->led.blink(Murli::Red, 3);
                    break;
                case WStype_CONNECTED:            
                    Serial.println("Connected to WebSocket!");
                    Murli::SocketClientPointer->led.blink(Murli::Green, 1);
                    break;
                case WStype_TEXT:
                    Murli::SocketClientPointer->lastLoopReceivedText = String((char*)payload);
                    break;
                default:
                    // Not interested in other cases
                    break;
            }
        };

        Serial.println("Connecting to socket '" + socketIp + "' ...");
        webSocket.begin(socketIp, 81, "/");
        webSocket.onEvent(webSocketEvent);
        webSocket.setReconnectInterval(5000);
    }

    void SocketClient::loop()
    {
        lastLoopReceivedText = String("");
        webSocket.loop();
    }
    
    String SocketClient::getReceivedText() const
    {
        return lastLoopReceivedText;
    }
}