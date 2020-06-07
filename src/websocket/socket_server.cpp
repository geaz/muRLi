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

    void SocketServer::broadcast(MurliCommand command)
    {
        webSocket.broadcastBIN((uint8_t*) &command, sizeof(command));
    }
}