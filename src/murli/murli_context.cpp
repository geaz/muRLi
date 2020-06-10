#include <Arduino.h>

#include "murli_context.hpp"
#include "../display/views/splash_view.cpp"
#include "states/no_mod_state.cpp"
#include "states/receive_write_state.cpp"

namespace Murli
{
    MurliContext::MurliContext()
    {
        _noModState = std::make_shared<NoModState>();
        currentState = _noModState;
    }

    void MurliContext::setup()
    {
        _display.init();
        _display.setView(std::make_shared<Murli::SplashView>());
        _display.setLeftStatus("Starting Mesh...");
        _display.loop();
        
        _wifi.startMesh();
        
        _display.setLeftStatus("WebSocket:");
        _display.setRightStatus("waiting");
    }

    void MurliContext::loop()
    {
        char heapString[10];
        itoa(ESP.getFreeHeap(), heapString, 10);
        _display.setLeftStatus(heapString);

        if(Serial.available() > 0
            && !writeRequested
            && isModInserted())
        {
            currentState = std::make_shared<ReceiveWriteState>();
        }
        // Reset state, if no MOD inserted
        else if(!isModInserted())
        {
            currentState = _noModState;
        }
        
        currentState->run(*this);
        _socketServer.loop();
        _display.loop();
    }

    bool MurliContext::isModInserted() const
    {
        return digitalRead(D6) == HIGH;
    }

    bool MurliContext::hasConnectedNodes() const
    { 
        return _wifi.hasConnectedNodes();
    }

    LED& MurliContext::getLed() { return _led; }
    Rom24LC32A& MurliContext::getRom() { return _rom; }
    Display& MurliContext::getDisplay() { return _display; }
    SocketServer& MurliContext::getSocketServer() { return _socketServer; }
}