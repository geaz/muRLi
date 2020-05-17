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

            bool isUpdaterAvailable();
            void connectToUpdater();
            
            void startMesh();
            bool isRootNode();
            IPAddress getParentIp();

        private:
            String ssid = SSID;
            IPAddress parentIp;
    };
}

#endif // WIFI_H