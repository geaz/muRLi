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
        
        _socketServer.onCommandReceived([this](MurliCommand command) { onSocketServerCommandReceived(command); });
        _socketServer.onMeshConnection([this]() { onSocketServerMeshConnection(); });

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
        }
    }

    void MurliContext::startMeshCount()
    {
        Serial.println("Connection state changed. Starting MESH_COUNT ...");
        if(_socketServer.connectedClients() == 0)
        {
            Serial.println("No clients connected!");
            Serial.printf("muRLi alone with %d LEDs :(\n", LED_COUNT);
            _meshLedCount = LED_COUNT;
        }
        else
        {
            MurliCommand command = { millis(), Murli::MESH_COUNT, 0, 0, LED_COUNT, 0 };

            _countData = { command, {}, 0, false };
            _socketServer.broadcast(command);
        }        
    }

    void MurliContext::onSocketServerMeshConnection()
    {
        startMeshCount();
    }

    void MurliContext::onSocketServerCommandReceived(Murli::MurliCommand command)
    {
        switch (command.command)
        {
            case Murli::MESH_CONNECTION:
                startMeshCount();
                break;
            case Murli::MESH_UPDATE:
                Serial.println("Got MESH_UPDATE!");
                _countData.answers++;
                // Save the retrieved MESH_UPDATE, if it is the first one or, if it has a larger LED count route
                if(_countData.answers == 1 || _countData.updateCommand.meshLEDCount < command.meshLEDCount)
                {
                    _countData.updateCommand = command;
                }
                // Send the MESH_UPDATE with the largest LED count/route to the parent,
                // if we retrieved answers of all clients
                if(_countData.answers == _socketServer.connectedClients())
                {
                    if(_meshLedCount < _countData.updateCommand.meshLEDCount)
                    {
                        Serial.println("New node connected. Resetting state ...");
                        currentState = std::make_shared<NoModState>();
                    }
                    _meshLedCount = _countData.updateCommand.meshLEDCount;
                    _countData = { {}, {}, 0, false };

                    Serial.printf("MESH_UPDATE done with %d LEDs on the longest route.\n", _meshLedCount);
                }
                break;
            default:
                // The socket server will never retrieve ANALYZER_UPDATE or MESH_COUNT events - MOD_DISTRIBUTED is handled in BraodcastModState
                break;
        }
    }

    LED& MurliContext::getLed() { return _led; }
    Rom24LC32A& MurliContext::getRom() { return _rom; }
    Display& MurliContext::getDisplay() { return _display; }
    uint16_t MurliContext::getMeshLedCount() { return _meshLedCount; }
    SocketServer& MurliContext::getSocketServer() { return _socketServer; }
}