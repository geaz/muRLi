#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <vector>
#include <WebSocketsServer.h>
#include "commands/server_commands.hpp"
#include "commands/client_commands.hpp"

namespace Murli
{
    typedef std::function<void()> MeshConnectionEvent;
    typedef std::function<void(Server::Command command)> ServerCommandEvent;

    class SocketServer
    {
        public:
            SocketServer();
            
            void loop();
            void broadcast(Client::Command command);
            void broadcastMod(std::string& mod);

            void addOnMeshConnection(MeshConnectionEvent event);
            void addOnCommandReceived(ServerCommandEvent event);

            void removeOnMeshConnection(MeshConnectionEvent event);
            void removeOnCommandReceived(ServerCommandEvent event);

            uint32_t getClientsCount();
        
        private:
            void serverEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
            bool allAnswerReceived(Server::Command receivedCommand);

            uint16_t _answers = 0;
            bool _modDistribution = false;
            Server::Command _receivedCommand;
            Client::Command _requestCommand;

            WebSocketsServer _webSocket = WebSocketsServer(81);
            std::vector<MeshConnectionEvent> _meshConnectionEvents;
            std::vector<ServerCommandEvent> _serverCommandEvents;
    };
}

#endif // SOCKETSERVER_H