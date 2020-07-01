#ifndef MURLINCONTEXT_H
#define MURLINCONTEXT_H

#include <memory>
#include "../hardware/led/led.hpp"
#include "../visualization/script_context.hpp"
#include "../network/mesh/murli_mesh.hpp"
#include "../network/websocket/socket_server.hpp"
#include "../network/websocket/socket_client.hpp"

namespace Murli
{
    static const uint8_t SleepSeconds = 60;

    class MurlinContext
    {
        public:
            void setup();
            void loop();

        private:
            void checkNewMod();
            void checkDistributeOrAnswer(Client::Command command, Server::CommandType answerCommandType);

            void onSocketClientCommandReceived(Client::Command command);
            void onSocketClientModReceived(std::string mod);
            void onSocketServerMeshConnection();
            void onSocketServerCommandReceived(Server::Command command);

            LED _led;
            MurliMesh _mesh;
            SocketServer _socketServer;
            SocketClient _socketClient;

            uint32_t _meshLedCount = 0;
            uint32_t _previousLedCount = 0;
            uint32_t _previousNodeCount = 0;

            bool _newMod = false;
            std::string _currentMod;
            uint64_t _lastUpdate = millis();
            std::unique_ptr<ScriptContext> _scriptContext = nullptr;
    };
}

#endif // MURLICONTEXT_H