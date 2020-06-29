#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <vector>
#include <WebSocketsClient.h>
#include "client_commands.hpp"
#include "server_commands.hpp"
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

            void addOnModReceived(MeshModEvent event);
            void addOnCommandReceived(ClientCommandEvent event);

            bool isConnected() const;
        
        private:
            void clientEvent(WStype_t type, uint8_t* payload, size_t length);

            WebSocketsClient _webSocket;            
            std::vector<MeshModEvent> _meshModEvents;
            std::vector<ClientCommandEvent> _clientCommandEvents;
            bool _isConnected = false;
    };
}

#endif // SOCKETCLIENT_H