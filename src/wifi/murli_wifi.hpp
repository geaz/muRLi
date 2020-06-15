#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

namespace Murli 
{
    static const String SSID = "muRLi Network";
    static const String Password = "muRLiSaysLetThereBeLight!";

    class MurliWifi
    {
        public:
            MurliWifi();
            
            void startMesh();
            bool tryJoinMesh();

            bool isConnected() const;
            bool hasConnectedNodes() const;
            IPAddress getParentIp() const;

        private:
            String _ssid = SSID;
            IPAddress _parentIp = IPAddress(0, 0, 0, 0);
    };
}

#endif // WIFI_H