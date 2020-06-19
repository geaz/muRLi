#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <WebSocketsClient.h>
#include "socket_server.hpp"
#include "../led/led.hpp"

namespace Murli
{
    
    typedef std::function<void(std::string mod)> MeshModEvent;

    class SocketClient
    {
        public:
            void start(String socketIp);
            void loop();
            void sendCommand(MurliCommand command);

            void onModReceived(MeshModEvent event);
            void onCommandReceived(MeshCommandEvent event);
            bool isConnected() const;
        
        private:
            void clientEvent(WStype_t type, uint8_t* payload, size_t length);

            WebSocketsClient _webSocket;            
            MeshModEvent _meshModEvent = nullptr;
            MeshCommandEvent _meshCommandEvent = nullptr;
            bool _isConnected = false;
    };
}

#endif // SOCKETCLIENT_H