#include "murli_mesh.hpp"

namespace Murli
{
    MurliMesh::MurliMesh()
    {
        WiFi.mode(WIFI_AP_STA);
        WiFi.setSleepMode(WIFI_NONE_SLEEP);
    }

    void MurliMesh::startMesh()
    {
        Serial.println("Starting mesh ...");
        
        _ssid = SSID + " #1";
        WiFi.softAPConfig(
            IPAddress(192, 168, 1, 1), 
            IPAddress(0, 0, 0, 0), 
            IPAddress(255, 255, 255, 0));
        WiFi.softAP(_ssid, Password, 1, false, 8);

        _localIp = WiFi.localIP();
        Serial.println("Node AP IP: " + WiFi.softAPIP().toString());
        Serial.println("Node Local IP: " + WiFi.localIP().toString());
    }

    bool MurliMesh::tryJoinMesh()
    {
        bool connected = false;

        Serial.println("Scanning for muRLi Nodes ...");
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
            }
            
            String ssid = WiFi.SSID(i);
            short foundNodeNr = ssid.substring(ssid.indexOf("#") + 1).toInt();
            if(foundNodeNr >= nodeNr) nodeNr = foundNodeNr + 1;
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

            _ssid = SSID + " #" + String(nodeNr);
            WiFi.softAPConfig(
                IPAddress(192, 168, nodeNr, 1), 
                IPAddress(0, 0, 0, 0), 
                IPAddress(255, 255, 255, 0));
            WiFi.softAP(_ssid, Password, 1, false, 8);

            Serial.println("Node AP IP: " + WiFi.softAPIP().toString());
            Serial.println("Node Local IP: " + WiFi.localIP().toString());
            connected = true;
            
            _parentIp = WiFi.gatewayIP();
            _localIp = WiFi.localIP();
        }  
        return connected;
    }

    bool MurliMesh::isConnected() const
    {
        return WiFi.isConnected();
    }

    bool MurliMesh::hasConnectedNodes() const
    {
        return WiFi.softAPgetStationNum() != 0;
    }

    IPAddress MurliMesh::getParentIp() const
    {
        return _parentIp;
    }

    IPAddress MurliMesh::getLocalIp() const
    {
        return _localIp;
    }
}