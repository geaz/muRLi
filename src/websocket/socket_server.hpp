#pragma once
#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <WebSocketsServer.h>

namespace Murli
{
    class SocketServer
    {
        public:
            SocketServer();
            
            void loop();
            void broadcast(String data);
        
        private:
            WebSocketsServer webSocket = WebSocketsServer(81);
    };
}

#endif // SOCKETSERVER_H