#ifndef MURLIDESKTOP_H
#define MURLIDESKTOP_H

#include "websocket.hpp"

namespace Murli
{
    namespace Desktop
    {
        class MurliDesktop
        {
            public:
                MurliDesktop(std::string url);

                void run();

            private:
                void onCommandReceived(Client::Command command);
                Server::Command createAnswer(Client::Command command, Server::CommandType answerCommandType);

                Websocket _websocket;
        };
    }
}

#endif // MURLIDESKTOP_H