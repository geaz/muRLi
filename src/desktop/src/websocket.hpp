#ifndef MURLIDESKTOPWEBSOCKET_H
#define MURLIDESKTOPWEBSOCKET_H

#include <vector>
#include <functional>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <client_commands.hpp>
#include <server_commands.hpp>
#include <event_registration.hpp>

namespace Murli
{
    namespace Desktop
    {
        typedef std::function<void(Client::Command)> CommandReceivedEvent;
        typedef std::function<void(bool)> ConnectionEvent;

        class Websocket
        {
            public:
                Websocket(const std::string url);

                void start();
                void send(Server::Command command);

                EventRegistration<ConnectionEvent> connectionEvents;
                EventRegistration<CommandReceivedEvent> commandEvents;

            private:
                void onMessage(const ix::WebSocketMessagePtr& msg);

                std::string _url;
                ix::WebSocket _socket;
        };
    }
}

#endif // MURLIDESKTOPWEBSOCKET_H