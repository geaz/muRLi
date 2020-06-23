#ifndef MURLINCONTEXT_H
#define MURLINCONTEXT_H

#include <memory>
#include "../led/led.hpp"
#include "../wifi/murli_wifi.hpp"
#include "../websocket/socket_server.hpp"
#include "../websocket/socket_client.hpp"
#include "../visualization/script_context.hpp"

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
            void checkDistributeOrAnswer(MurliCommand command, Command answerCommandType);

            void onSocketClientCommandReceived(MurliCommand command);
            void onSocketClientModReceived(std::string mod);
            void onSocketServerMeshConnection();
            void onSocketServerModDistributed(MurliCommand command);
            void onSocketServerCommandReceived(MurliCommand command);

            LED _led;
            MurliWifi _wifi;
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