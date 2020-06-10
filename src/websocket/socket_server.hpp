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
        ColorFrame colorFrame = { Instant, Black, Black };

        Color getNewNodeColor(const Color currentColor, const bool isBase, const bool hasConnectedNodes)
        {
            Color newColor = currentColor.getFadedBlack(64);
            if(command == SET && (colorFrame.second.isBlack() || isBase))
            {
                newColor = colorFrame.first;
            }
            else if(command == SET && hasConnectedNodes)
            {
                newColor = Color::blend(colorFrame.first, colorFrame.second);
                colorFrame = { colorFrame.mode, newColor, colorFrame.second };
            }
            else if(command == SET)
            {
                newColor = colorFrame.second;
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