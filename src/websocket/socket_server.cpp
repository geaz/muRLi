#include "socket_server.hpp"

namespace Murli
{
    SocketServer::SocketServer()
    {
        webSocket.begin();
        webSocket.enableHeartbeat(2000, 1500, 1);
    }

    void SocketServer::loop()
    {
        webSocket.loop();
    }

    void SocketServer::broadcast(String data)
    {
        webSocket.broadcastTXT(data);
    }
}