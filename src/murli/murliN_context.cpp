#include "murliN_context.hpp"

namespace Murli
{
    void MurlinContext::setup()
    {
        _socketServer.onMeshConnection([this]() { onSocketServerMeshConnection(); });
        _socketServer.onCommandReceived([this](MurliCommand command) { onSocketServerCommandReceived(command); });
        _socketClient.onCommandReceived([this](MurliCommand command) { onSocketClientCommandReceived(command); });
        _socketClient.onModReceived([this](std::string mod) {onSocketClientModReceived(mod); });

        if(_wifi.tryJoinMesh())
        {
            Serial.println("Connected to muRLi mesh! Starting operation ...");
            _socketClient.start(_wifi.getParentIp().toString());
        }
        else
        {
            Serial.println("muRLi not found - Going to sleep ...");
            ESP.deepSleep(SleepSeconds * 1000000);
        }
    }

    void MurlinContext::loop()
    {
        checkNewMod();
        
        _socketClient.loop();
        _socketServer.loop();
        _led.loop();
        if(!_wifi.isConnected())
        {
            Serial.println("Websocket and WiFi disconnected! Going to sleep ...");
            _led.setAllLeds(Murli::Black);
            _led.loop();
            ESP.deepSleep(SleepSeconds * 1000000);
        }
    }

    void MurlinContext::checkNewMod()
    {
        if(_newMod){
            Serial.println("Received new mod. Creating new ScriptContext ...");
            _scriptContext = std::unique_ptr<Murli::ScriptContext>(new ScriptContext(_led, _currentMod));
            _newMod = false;
        }
    }

    void MurlinContext::onSocketServerMeshConnection()
    {
        Serial.println("Got a new connection. Informing parent node ...");
        Murli::MurliCommand command = { millis(), Murli::MESH_CONNECTION, 0, 0, 0, 0 };
        _socketClient.sendCommand(command);
    }

    void MurlinContext::onSocketServerCommandReceived(Murli::MurliCommand command)
    {
        switch (command.command)
        {
            // Send a MESH_CONNECTION up until the root is reached
            case Murli::MESH_CONNECTION:
                _socketClient.sendCommand(command);
                break;
            case Murli::MESH_UPDATE:
                _currentCountData.answers++;
                // Save the retrieved MESH_UPDATE, if it is the first one or, if it has a larger LED count route
                if(_currentCountData.answers == 1 || _currentCountData.updateCommand.meshLedCount < command.meshLedCount)
                {
                    _currentCountData.updateCommand = command;
                }
                // Send the MESH_UPDATE with the largest LED count/route to the parent,
                // if we retrieved answers of all clients
                if(_currentCountData.answers == _socketServer.connectedClients())
                {
                    _socketClient.sendCommand(_currentCountData.updateCommand);
                    _currentCountData = { {}, {}, 0, false };
                }
                break;
            default:
                // The socket server will never retrieve ANALYZER_UPDATE or MESH_COUNT events
                break;
        }
    }

    void MurlinContext::onSocketClientCommandReceived(Murli::MurliCommand command)
    {
        switch (command.command)
        {
            case Murli::MESH_COUNT:
                command.meshLedCount += LED_COUNT;
                // If the current node is the last one (no clients connected)
                // Send the LED count back to the parent
                if(_socketServer.connectedClients() == 0)
                {
                    command.command = Murli::MESH_UPDATE;
                    _socketClient.sendCommand(command);
                }
                // Only forward the current MESH_COUNT command, if
                // no other MESH_COUNT is active or the the command
                // was requested after the current one (a new node connected to the network during an active MESH_COUNT)
                else if(!_currentCountData.active || _currentCountData.countCommand.id < command.id)
                {
                    _currentCountData = { command, {}, 0, true };
                    _socketServer.broadcast(command);
                }
                break;
            case Murli::ANALYZER_UPDATE:
                if(_scriptContext != nullptr)
                {
                    uint32_t delta = millis() - _lastUpdate;
                    _lastUpdate = millis();

                    _scriptContext->updateLedInfo(command.previousNodeCount, command.previousLedCount, command.meshLedCount);
                    _scriptContext->updateAnalyzerResult(command.volume, command.frequency);
                    _scriptContext->run(delta);

                    command.previousLedCount += LED_COUNT;
                    command.previousNodeCount++;
                    _socketServer.broadcast(command);
                }
                break;
            default:
                // The SocketClient does not have to handle MESH_CONNECTION or MESH_UPDATE events
                break;
        }
    }

    void MurlinContext::onSocketClientModReceived(std::string mod)
    {
        // DON'T create the ScriptContext in the callback
        // Unexpected errors will occure!
        _newMod = true;
        _currentMod = mod;

        if(_socketServer.connectedClients() == 0)
        {
            Serial.println("End of route - Sending MOD_DISTRIBUTED");
            Murli::MurliCommand command = { millis(), Murli::MOD_DISTRIBUTED, 0, 0, 0, 0 };
            _socketClient.sendCommand(command);
        }
    }
}