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
        size_t commandSize = sizeof(command);
        uint8_t serializedCommand[commandSize];
        memcpy(&serializedCommand, &command, commandSize);

        webSocket.broadcastBIN(&serializedCommand[0], commandSize);
    }
}