#ifndef MURLIDESKTOPWEBSOCKET_H
#define MURLIDESKTOPWEBSOCKET_H

#include <vector>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <client_commands.hpp>
#include <server_commands.hpp>

namespace Murli
{
    namespace Desktop
    {
        typedef std::function<void(Client::Command)> CommandReceivedEvent;

        class Websocket
        {
            public:
                Websocket(const std::string url);

                void start();
                void send(Server::Command command);
                void addOnCommandReceived(CommandReceivedEvent event);

            private:
                void onMessage(const ix::WebSocketMessagePtr& msg);

                std::string _url;
                ix::WebSocket _socket;
                std::vector<CommandReceivedEvent> _commandReceivedEvents;
        };
    }
}

#endif // MURLIDESKTOPWEBSOCKET_H