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

#include "led/led.hpp"
#include "wifi/murli_wifi.hpp"
#include "websocket/socket_client.hpp"
#include "websocket/socket_server.hpp"

uint8_t sleepSeconds = 60;

Murli::LED led;
Murli::MurliWifi wifi;
Murli::SocketClient socketClient(led);
Murli::SocketServer socketServer;

void setup()
{
    Serial.begin(74880);
    Serial.println("Starting muRLiN ...");

    pinMode(Murli::LedDataPin, OUTPUT);
    led.setColor(Murli::Black);

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
    if(socketClient.isConnected() && socketClient.hasNewCommand())
    {
        Murli::MurliCommand currentCommand = socketClient.getNewCommand();
        
        led.setColor(currentCommand.getNewNodeColor(led.getColor(), false, wifi.hasConnectedNodes()));
        socketServer.broadcast(currentCommand);
    }
    else if(!wifi.isConnected())
    {
        Serial.println("Websocket and WiFi disconnected! Going to sleep ...");
        led.setColor(Murli::Black);
        ESP.deepSleep(sleepSeconds * 1000000);
    }
}


#endif