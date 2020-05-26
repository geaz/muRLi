#include "murli_wifi.hpp"

namespace Murli
{
    MurliWifi::MurliWifi()
    {
        WiFi.disconnect();
        WiFi.mode(WIFI_AP_STA);
        WiFi.setSleepMode(WIFI_NONE_SLEEP);
    }

    void MurliWifi::startMesh()
    {
        Serial.println("Starting mesh ...");

        ssid = SSID + " #1";
        WiFi.softAPConfig(
            IPAddress(192, 168, 1, 1), 
            IPAddress(0, 0, 0, 0), 
            IPAddress(255, 255, 255, 0));
        WiFi.softAP(ssid, Password, 1, false, 8);

        Serial.println("Node AP IP: " + WiFi.softAPIP().toString());
        Serial.println("Node Local IP: " + WiFi.localIP().toString());

      /*  Serial.println("Scanning for muRLi Nodes ...");
        unsigned char foundNetworkCount = WiFi.scanNetworks();
        unsigned char nodeNr = 1;
        short nearestNode = -1;
        for (int i = 0; i < foundNetworkCount; ++i)
        {
            if(WiFi.SSID(i).startsWith(SSID))
            {
                if (nearestNode == -1 ||
                    (nearestNode != -1 && WiFi.RSSI(i) > WiFi.RSSI(nearestNode)))
                {
                    nearestNode = i;
                }
                nodeNr++;
            }
        }

        if(nearestNode != -1) 
        {
            Serial.print("Connecting to '" + WiFi.SSID(nearestNode) + "'");
            WiFi.begin(WiFi.SSID(nearestNode), Password);     
            while (WiFi.status() != WL_CONNECTED)
            {
                delay(500);
                Serial.print(".");
            }       
            Serial.println(" Connected!");
            parentIp = IPAddress(192, 168, nodeNr - 1, 1);
        }  
        
        ssid = SSID + " #" + String(nodeNr);
        WiFi.softAPConfig(
            IPAddress(192, 168, nodeNr, 1), 
            IPAddress(0, 0, 0, 0), 
            IPAddress(255, 255, 255, 0));
        WiFi.softAP(ssid, Password, 1, false, 8);

        Serial.println("Node AP IP: " + WiFi.softAPIP().toString());
        Serial.println("Node Local IP: " + WiFi.localIP().toString());*/
    }

    bool MurliWifi::isRootNode() 
    { 
        return ssid.endsWith(" #1"); 
    }

    IPAddress MurliWifi::getParentIp()
    {
        return parentIp;
    }
}