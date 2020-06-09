#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <WebSocketsClient.h>
#include "socket_server.hpp"
#include "../led/led.hpp"

namespace Murli
{
    class SocketClient
    {
        public:
            SocketClient(LED& led);

            void start(String socketIp);
            void loop();
            
            bool isConnected() const;
            bool hasNewCommand() const;
            MurliCommand getNewCommand();
        
        private:
            LED& _led;
            WebSocketsClient _webSocket;
            MurliCommand _newReceivedCommand;
            
            bool _isConnected = false;
            bool _hasNewCommand = false;
    };

    // SocketClient Pointer for Socket callbacks
    extern SocketClient* SocketClientPointer;
}

#endif // SOCKETCLIENT_H