#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <WebSocketsServer.h>
#include "commands/server_commands.hpp"
#include "commands/client_commands.hpp"
#include "event_registration.hpp"

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

            uint32_t getClientsCount();

            EventRegistration<MeshConnectionEvent> meshConnectionEvents;
            EventRegistration<ServerCommandEvent> serverCommandEvents;
        
        private:
            void serverEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
            bool allAnswerReceived(Server::Command receivedCommand);

            uint16_t _answers = 0;
            bool _modDistribution = false;
            Server::Command _receivedCommand;
            Client::Command _requestCommand;

            WebSocketsServer _webSocket = WebSocketsServer(81);
    };
}

#endif // SOCKETSERVER_H