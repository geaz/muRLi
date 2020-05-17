#pragma once
#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <WebSocketsClient.h>
#include "../led/led.hpp"

namespace Murli
{
    class SocketClient
    {
        public:
            SocketClient(LED& led);

            void start(String socketIp);
            void loop();
            
            String getReceivedText() const;
        
        private:
            LED& led;
            WebSocketsClient webSocket;
            String lastLoopReceivedText;
    };

    // SocketClient Pointer for Socket callbacks
    extern SocketClient* SocketClientPointer;
}

#endif // SOCKETCLIENT_H