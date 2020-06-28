#include "murliN_context.hpp"

namespace Murli
{
    void MurlinContext::setup()
    {
        _socketServer.onMeshConnection([this]() { onSocketServerMeshConnection(); });
        _socketServer.onModDistributed([this](MurliCommand command) { onSocketServerModDistributed(command); });
        _socketServer.onCommandReceived([this](MurliCommand command) { onSocketServerCommandReceived(command); });
        _socketClient.onCommandReceived([this](MurliCommand command) { onSocketClientCommandReceived(command); });
        _socketClient.onModReceived([this](std::string mod) {onSocketClientModReceived(mod); });

        if(_mesh.tryJoinMesh())
        {
            Serial.println("Connected to muRLi mesh! Starting operation ...");
            _socketClient.start(_mesh.getParentIp().toString());
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
        if(!_mesh.isConnected())
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
            _led.clearGroups();
            _scriptContext = std::unique_ptr<Murli::ScriptContext>(new ScriptContext(_led, _currentMod));
            _scriptContext->updateLedInfo(_previousNodeCount, _previousLedCount, _meshLedCount);
            _scriptContext->init();
            _newMod = false;
        }
    }

    void MurlinContext::checkDistributeOrAnswer(MurliCommand command, Command answerCommandType)
    {
        if(_socketServer.connectedClients() == 0)
        {
            command.command = answerCommandType;
            _socketClient.sendCommand(command);
        }
        else { _socketServer.broadcast(command); }
    }

    void MurlinContext::onSocketClientCommandReceived(Murli::MurliCommand command)
    {
        switch (command.command)
        {
            case Murli::MESH_COUNT_REQUEST:
                Serial.println("MESH_COUNT_REQUEST received...");
                command.meshLedCount += LED_COUNT;
                checkDistributeOrAnswer(command, Murli::MESH_COUNTED);
                break;
            case Murli::MESH_UPDATE:
                _meshLedCount = command.meshLedCount;
                _previousLedCount = command.previousLedCount;
                _previousNodeCount = command.previousNodeCount;

                command.previousLedCount += LED_COUNT;
                command.previousNodeCount++;
                
                checkDistributeOrAnswer(command, Murli::MESH_UPDATED);
                break;
            case Murli::MOD_REMOVED:
                _led.setAllLeds(Murli::Black);
                _socketServer.broadcast(command);
                break;
            case Murli::ANALYZER_UPDATE:
                if(_scriptContext != nullptr)
                {
                    uint32_t delta = millis() - _lastUpdate;
                    _lastUpdate = millis();
                    
                    _scriptContext->updateAnalyzerResult(command.volume, command.frequency);
                    _scriptContext->run(delta);

                    command.previousLedCount += LED_COUNT;
                    command.previousNodeCount++;
                    _socketServer.broadcast(command);
                }
                break;
            default:
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
        else _socketServer.broadcastMod(mod);
    }    

    void MurlinContext::onSocketServerMeshConnection()
    {
        Serial.println("Got a new connection. Informing parent node ...");
        Murli::MurliCommand command = { millis(), Murli::MESH_CONNECTION, 0, 0, 0, 0 };
        _socketClient.sendCommand(command);
    }

    void MurlinContext::onSocketServerCommandReceived(MurliCommand command)
    {
        switch (command.command)
        {
            case Murli::MESH_CONNECTION:
            case Murli::MESH_COUNTED:
            case Murli::MESH_UPDATED:
                _socketClient.sendCommand(command);
                break;
            default:
                break;
        }
    }

    void MurlinContext::onSocketServerModDistributed(MurliCommand command)
    {
        _socketClient.sendCommand(command);
    }
}