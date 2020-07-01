#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <WebSocketsClient.h>
#include "commands/client_commands.hpp"
#include "commands/server_commands.hpp"
#include "event_registration.hpp"
#include "../led/led.hpp"

namespace Murli
{    
    typedef std::function<void(std::string mod)> MeshModEvent;
    typedef std::function<void(Client::Command command)> ClientCommandEvent;

    class SocketClient
    {
        public:
            void start(String socketIp);
            void loop();
            void sendCommand(Server::Command command);

            bool isConnected() const;

            EventRegistration<MeshModEvent> meshModEvents;
            EventRegistration<ClientCommandEvent> clientCommandEvents;
        
        private:
            void clientEvent(WStype_t type, uint8_t* payload, size_t length);

            WebSocketsClient _webSocket;
            bool _isConnected = false;
    };
}

#endif // SOCKETCLIENT_H