#include "murliN_context.hpp"

namespace Murli
{
    void MurlinContext::setup()
    {
        _socketServer.addOnMeshConnection([this]() { onSocketServerMeshConnection(); });
        _socketServer.addOnCommandReceived([this](Server::Command command) { onSocketServerCommandReceived(command); });
        _socketClient.addOnCommandReceived([this](Client::Command command) { onSocketClientCommandReceived(command); });
        _socketClient.addOnModReceived([this](std::string mod) {onSocketClientModReceived(mod); });
        
        WiFi.disconnect();
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

    void MurlinContext::checkDistributeOrAnswer(Client::Command command, Server::CommandType answerCommandType)
    {
        if(_socketServer.getClientsCount() == 0)
        {
            Server::Command answerCommand = { command.id, answerCommandType };
            if(answerCommandType == Server::MESH_COUNTED)
            {
                Server::CountedCommand counted = { command.countCommand.meshLedCount };
                answerCommand.countedCommand = counted;
            }
            _socketClient.sendCommand(answerCommand);
        }
        else { _socketServer.broadcast(command); }
    }

    void MurlinContext::onSocketClientCommandReceived(Client::Command command)
    {
        switch (command.commandType)
        {
            case Client::MESH_COUNT_REQUEST:
                Serial.println("MESH_COUNT_REQUEST received...");
                command.countCommand.meshLedCount += LED_COUNT;
                checkDistributeOrAnswer(command, Server::MESH_COUNTED);
                break;
            case Client::MESH_UPDATE:
                _meshLedCount = command.countCommand.meshLedCount;
                _previousLedCount = command.countCommand.previousLedCount;
                _previousNodeCount = command.countCommand.previousNodeCount;

                command.countCommand.previousLedCount += LED_COUNT;
                command.countCommand.previousNodeCount++;
                
                checkDistributeOrAnswer(command, Server::MESH_UPDATED);
                break;
            case Client::MOD_REMOVED:
                _led.setAllLeds(Murli::Black);
                _socketServer.broadcast(command);
                break;
            case Client::ANALYZER_UPDATE:
                if(_scriptContext != nullptr)
                {
                    uint32_t delta = millis() - _lastUpdate;
                    _lastUpdate = millis();
                    
                    _scriptContext->updateAnalyzerResult(command.analyzerCommand.volume, command.analyzerCommand.frequency);
                    _scriptContext->run(delta);                    
                    _socketServer.broadcast(command);
                }
                break;
        }
    }

    void MurlinContext::onSocketClientModReceived(std::string mod)
    {
        // DON'T create the ScriptContext in the callback
        // Unexpected errors will occure!
        _newMod = true;
        _currentMod = mod;
        if(_socketServer.getClientsCount() == 0)
        {
            Serial.println("End of route - Sending MOD_DISTRIBUTED");
            Server::Command command = { millis(), Server::MOD_DISTRIBUTED };
            _socketClient.sendCommand(command);
        }
        else _socketServer.broadcastMod(mod);
    }    

    void MurlinContext::onSocketServerMeshConnection()
    {
        Serial.println("Got a new connection. Informing parent node ...");
        Server::Command command = { millis(), Server::MESH_CONNECTION };
        _socketClient.sendCommand(command);
    }

    void MurlinContext::onSocketServerCommandReceived(Server::Command command)
    {
        switch (command.commandType)
        {
            case Server::MESH_CONNECTION:
            case Server::MESH_COUNTED:
            case Server::MESH_UPDATED:
            case Server::MOD_DISTRIBUTED:
                _socketClient.sendCommand(command);
                break;
        }
    }
}