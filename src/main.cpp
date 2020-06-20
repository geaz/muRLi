#ifdef BUILD_MURLI

#include "murli/murli_context.hpp"

Murli::MurliContext murliContext;

void setup()
{
    Serial.begin(74880);
    Serial.println("Starting muRLi...");
    Wire.begin();

    murliContext.setup();
    
    pinMode(A0, INPUT);         // Mic Input
    pinMode(D6, INPUT);         // Check if module inserted
    pinMode(LED_PIN, OUTPUT);   // LED
}

void loop() 
{
    murliContext.loop();
}

#elif BUILD_MURLINODE

#include <memory>
#include <vector>
#include "led/led.hpp"
#include "wifi/murli_wifi.hpp"
#include "visualization/script_context.hpp"
#include "websocket/murli_command.hpp"
#include "websocket/socket_client.hpp"
#include "websocket/socket_server.hpp"

uint8_t sleepSeconds = 60;
uint32_t lastUpdate = millis();

Murli::LED led;
Murli::MurliWifi wifi;
Murli::SocketClient socketClient;
Murli::SocketServer socketServer;

Murli::MurliCountData currentCountData = { {}, {}, 0, false };
std::shared_ptr<Murli::ScriptContext> scriptContext = nullptr;

void onSocketServerMeshConnection();
void onSocketServerCommandReceived(Murli::MurliCommand command);
void onSocketClientCommandReceived(Murli::MurliCommand command);
void onSocketClientModReceived(std::string mod);

void setup()
{
    Serial.begin(74880);
    Serial.println("Starting muRLiN ...");

    pinMode(LED_PIN, OUTPUT);

    socketServer.onMeshConnection(onSocketServerMeshConnection);
    socketServer.onCommandReceived(onSocketServerCommandReceived);
    socketClient.onCommandReceived(onSocketClientCommandReceived);
    socketClient.onModReceived(onSocketClientModReceived);

    if(wifi.tryJoinMesh())
    {
        Serial.println("Connected to muRLi mesh! Starting operation ...");
        socketClient.start(wifi.getParentIp().toString());
    }
    else
    {
        Serial.println("muRLi not found - Going to sleep ...");
        ESP.deepSleep(sleepSeconds * 1000000);
    }
}

void loop()
{
    socketClient.loop();
    socketServer.loop();
    led.loop();
    if(!wifi.isConnected())
    {
        Serial.println("Websocket and WiFi disconnected! Going to sleep ...");
        led.setAllLeds(Murli::Black);
        ESP.deepSleep(sleepSeconds * 1000000);
    }
}

void onSocketServerMeshConnection()
{
    Serial.println("Got a new connection. Informing parent node ...");
    Murli::MurliCommand command = { millis(), Murli::MESH_CONNECTION, 0, 0, 0, 0 };
    socketClient.sendCommand(command);
}

void onSocketServerCommandReceived(Murli::MurliCommand command)
{
    switch (command.command)
    {
        // Send a MESH_CONNECTION up until the root is reached
        case Murli::MESH_CONNECTION:
            socketClient.sendCommand(command);
            break;
        case Murli::MESH_UPDATE:
            currentCountData.answers++;
            // Save the retrieved MESH_UPDATE, if it is the first one or, if it has a larger LED count route
            if(currentCountData.answers == 1 || currentCountData.updateCommand.meshLEDCount < command.meshLEDCount)
            {
                currentCountData.updateCommand = command;
            }
            // Send the MESH_UPDATE with the largest LED count/route to the parent,
            // if we retrieved answers of all clients
            if(currentCountData.answers == socketServer.connectedClients())
            {
                socketClient.sendCommand(currentCountData.updateCommand);
                currentCountData = { {}, {}, 0, false };
            }
            break;
        default:
            // The socket server will never retrieve ANALYZER_UPDATE or MESH_COUNT events
            break;
    }
}

void onSocketClientCommandReceived(Murli::MurliCommand command)
{
    switch (command.command)
    {
        case Murli::MESH_COUNT:
            command.meshLEDCount += LED_COUNT;
            // If the current node is the last one (no clients connected)
            // Send the LED count back to the parent
            if(socketServer.connectedClients() == 0)
            {
                command.command = Murli::MESH_UPDATE;
                socketClient.sendCommand(command);
            }
            // Only forward the current MESH_COUNT command, if
            // no other MESH_COUNT is active or the the command
            // was requested after the current one (a new node connected to the network during an active MESH_COUNT)
            else if(!currentCountData.active || currentCountData.countCommand.id < command.id)
            {
                currentCountData = { command, {}, 0, true };
                socketServer.broadcast(command);
            }
            break;
        case Murli::ANALYZER_UPDATE:
            if(scriptContext != nullptr)
            {
                uint32_t delta = millis() - lastUpdate;
                lastUpdate = millis();

                scriptContext->updateLedInfo(command.previousLEDCount, command.meshLEDCount);
                scriptContext->updateAnalyzerResult(command.volume, command.frequency);
                scriptContext->run(delta);

                command.previousLEDCount += LED_COUNT;
                socketServer.broadcast(command);
            }
            break;
        default:
            // The SocketClient does not have to handle MESH_CONNECTION or MESH_UPDATE events
            break;
    }
}

void onSocketClientModReceived(std::string mod)
{
    Serial.println("Received new mod. Creating new ScriptContext ...");
    scriptContext = std::make_shared<Murli::ScriptContext>(led, mod);
    if(socketServer.connectedClients() == 0)
    {
        Serial.println("End of route - Sending MOD_DISTRIBUTED");
        Murli::MurliCommand command = { millis(), Murli::MOD_DISTRIBUTED, 0, 0, 0, 0 };
        socketClient.sendCommand(command);
    }
}

#endif