#include <Arduino.h>

#include "murli_context.hpp"
#include "../display/views/splash_view.cpp"
#include "states/no_mod_state.cpp"
#include "states/receive_length_state.cpp"

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
    }

    void MurliContext::loop()
    {
        char heapString[10];
        itoa(ESP.getFreeHeap(), heapString, 10);
        _display.setLeftStatus(heapString);

        checkModuleInserted();
        checkWriteRequest();
        
        currentState->run(*this);
        _socketServer.loop();
        _led.loop();
        _display.loop();
    }

    bool MurliContext::isModInserted() const
    {
        return digitalRead(D6) == HIGH;
    }

    void MurliContext::checkModuleInserted()
    {
        if(!isModInserted()) currentState = _noModState;
    }

    void MurliContext::checkWriteRequest()
    {
        if(Serial.available() > 0
            && !writeRequested
            && isModInserted())
        {
            int incomingByte = Serial.read();
            if(incomingByte == 30)
            {
                Serial.write(30);
                writeRequested = true;
                currentState = std::make_shared<ReceiveLengthState>();
            }
            else currentState = std::make_shared<NoModState>();
        }
    }

    LED& MurliContext::getLed() { return _led; }
    Rom24LC32A& MurliContext::getRom() { return _rom; }
    Display& MurliContext::getDisplay() { return _display; }
    uint16_t MurliContext::getMeshLedCount() { return _meshLedCount; }
    SocketServer& MurliContext::getSocketServer() { return _socketServer; }
}