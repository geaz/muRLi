#ifndef MURLICONTEXT_H
#define MURLICONTEXT_H

#include <memory>

#include "state.hpp"
#include "../led/led.hpp"
#include "../wifi/murli_wifi.hpp"
#include "../display/display.hpp"
#include "../rom/rom_24LC32A.hpp"
#include "../websocket/socket_server.hpp"

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
            uint16_t getMeshLedCount();
            SocketServer& getSocketServer();

            bool writeRequested = false;
            std::shared_ptr<State> currentState;

        private:
            void checkModuleInserted();
            void checkWriteRequest();

            LED _led;
            MurliWifi _wifi;
            Display _display;
            SocketServer _socketServer;
            Rom24LC32A _rom = Rom24LC32A(0x50);

            uint16_t _meshLedCount = LED_COUNT;
            std::shared_ptr<State> _noModState;
    };
}

#endif // MURLICONTEXT_H