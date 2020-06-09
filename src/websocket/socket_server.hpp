#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <WebSocketsServer.h>
#include "../led/color.hpp"

namespace Murli
{
    enum Command
    {
        SET,
        FADE
    };

    struct MurliCommand
    {
        Command command;
        ColorFrame colorFrame = { Black, Black };

        Color getNewNodeColor(const Color currentColor)
        {
            Color newColor = currentColor.getFadedBlack(64);
            if(command == SET && colorFrame.second.isBlack())
            {
                newColor = colorFrame.first;
            }
            else if(command == SET)
            {
                newColor = Color::blend(colorFrame.first, colorFrame.second);
                colorFrame = { newColor, colorFrame.second };
            }
            return newColor;
        }
    };

    class SocketServer
    {
        public:
            SocketServer();
            
            void loop();
            void broadcast(MurliCommand command);
        
        private:
            WebSocketsServer webSocket = WebSocketsServer(81);
    };
}

#endif // SOCKETSERVER_H