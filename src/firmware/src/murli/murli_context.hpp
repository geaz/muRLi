#ifndef MURLICONTEXT_H
#define MURLICONTEXT_H

#include <memory>
#include "state.hpp"
#include "../hardware/led/led.hpp"
#include "../hardware/display/display.hpp"
#include "../hardware/rom/rom_24LC32A.hpp"
#include "../network/mesh/murli_mesh.hpp"
#include "../network/websocket/socket_server.hpp"
#include "../network/webserver/murli_web.hpp"

namespace Murli
{
    class MurliContext
    {
        public:
            MurliContext();

            void setup();
            void loop();

            bool isModInserted() const;

            LED& getLed();
            Rom24LC32A& getRom();
            Display& getDisplay();
            uint32_t getMeshLedCount();
            SocketServer& getSocketServer();
            IPAddress getLocalIp();

            bool writeRequested = false;
            std::shared_ptr<State> currentState;

        private:
            void checkModuleInserted();
            void checkWriteRequest();

            void startMeshCount();
            void onSocketServerMeshConnection();
            void onSocketServerCommandReceived(Server::Command command);

            LED _led;
            MurliWeb _web;
            MurliMesh _mesh;
            Display _display;
            SocketServer _socketServer;
            Rom24LC32A _rom = Rom24LC32A(0x50);

            uint32_t _meshLedCount = LED_COUNT;
            std::shared_ptr<State> _noModState;
    };
}

#endif // MURLICONTEXT_H