#include "murli_context.hpp"
#include "states/no_mod_state.cpp"
#include "states/receive_length_state.cpp"
#include "../views/splash_view.cpp"

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
        _display.loop();
        
        _socketServer
            .serverCommandEvents
            .addEventHandler([this](Server::Command command) { onSocketServerCommandReceived(command); });
        _socketServer
            .meshConnectionEvents
            .addEventHandler([this]() { onSocketServerMeshConnection(); });

        WiFi.disconnect();
        _web.connectNetwork();
        _mesh.startMesh();
    }

    void MurliContext::loop()
    {
        checkModuleInserted();
        checkWriteRequest();
        
        currentState->run(*this);
        _socketServer.loop();
        _led.loop();
        _web.loop();
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
        Serial.println("Connection state changed. Starting MESH_COUNT_REQUEST ...");
        if(_socketServer.getClientsCount() == 0)
        {
            Serial.println("No clients connected!");
            Serial.printf("muRLi alone with %d LEDs :(\n", LED_COUNT);
            _meshLedCount = LED_COUNT;
        }
        else
        {
            Client::CountCommand countCommand = { LED_COUNT, LED_COUNT, 1 };
            Client::Command command = { millis(), Client::MESH_COUNT_REQUEST };
            command.countCommand = countCommand;
            
            _socketServer.broadcast(command);
        }
    }

    void MurliContext::onSocketServerMeshConnection()
    {
        startMeshCount();
    }

    void MurliContext::onSocketServerCommandReceived(Server::Command command)
    {
        Client::Command updateCommand;
        Client::CountCommand countCommand;
        switch (command.commandType)
        {
            case Server::MESH_CONNECTION:
                startMeshCount();
                break;
            case Server::MESH_COUNTED:
                _meshLedCount = command.countedCommand.meshLedCount;
                Serial.printf("MESH_COUNT done with %d LEDs on the longest route.\n", _meshLedCount);
                Serial.println("Distributing result of count ...");

                countCommand = { _meshLedCount, LED_COUNT, 1 };
                updateCommand = { millis(), Client::MESH_UPDATE };
                updateCommand.countCommand = countCommand;
                _socketServer.broadcast(updateCommand);
                break;
            case Server::MESH_UPDATED:
                Serial.println("Mesh updated - Resetting state ...");
                currentState = std::make_shared<NoModState>();
                break;
            case Server::EXTERNAL_ANALYZER:
                Serial.println("External");
                Serial.printf("%d %d\n", command.externalAnalyzerCommand.volume, command.externalAnalyzerCommand.frequency);
                break;
        }
    }

    LED& MurliContext::getLed() { return _led; }
    Rom24LC32A& MurliContext::getRom() { return _rom; }
    Display& MurliContext::getDisplay() { return _display; }
    uint32_t MurliContext::getMeshLedCount() { return _meshLedCount; }
    SocketServer& MurliContext::getSocketServer() { return _socketServer; }
}