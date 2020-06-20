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
            void onSocketServerMeshConnection();
            void onSocketServerCommandReceived(Murli::MurliCommand command);
            void onSocketClientCommandReceived(Murli::MurliCommand command);
            void onSocketClientModReceived(std::string mod);

            LED _led;
            MurliWifi _wifi;
            SocketServer _socketServer;
            SocketClient _socketClient;

            bool _newMod = false;
            std::string _currentMod;
            uint32_t _lastUpdate = millis();            
            MurliCountData _currentCountData = { {}, {}, 0, false };
            std::unique_ptr<ScriptContext> _scriptContext = nullptr;
    };
}

#endif // MURLICONTEXT_H