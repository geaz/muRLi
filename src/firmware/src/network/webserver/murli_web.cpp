#include <EEPROM.h>
#include "murli_web.hpp"
#include "pages/index.h"

namespace Murli
{
    MurliWeb::MurliWeb() : _server(80)
    {
        EEPROM.begin(512);
        WiFi.mode(WIFI_AP_STA);
        WiFi.setSleepMode(WIFI_NONE_SLEEP);

        _server.on("/", [this]() { _server.send(200, "text/html", IndexPage); });
        _server.on("/setSSID", [this]() { saveSSID(); });
        _server.begin();
    }

    void MurliWeb::loop()
    {
        _server.handleClient();
    }

    void MurliWeb::connectNetwork()
    {
        Serial.println("Connecting to Network ...");
        
        String ssid;
        for (int i = 0; i < 32; ++i)
        {
            ssid += char(EEPROM.read(i));
        }
        Serial.printf("SSID: %s\n", ssid.c_str());
        
        String password = "";
        for (int i = 32; i < 96; ++i)
        {
            password += char(EEPROM.read(i));
        }
        Serial.printf("Password: %s\n", password.c_str());
        
        if (ssid.length() > 1 ) 
        {
            WiFi.begin(ssid, password);
            int waitCounter = 0;
            Serial.print("Waiting for Wifi to connect ");
            while (WiFi.status() != WL_CONNECTED && waitCounter < 20) 
            {
                delay(500);
                Serial.print(".");
                waitCounter++;
            }
            
            if(WiFi.status() != WL_CONNECTED)
            {
                Serial.println("Couldn't connect to network!");
                WiFi.disconnect();
            }
            else { Serial.println("Connected!"); }
        }
    }

    void MurliWeb::saveSSID()
    {
        String ssid = _server.arg("ssid");
        String password = _server.arg("pass");

        if (ssid.length() > 0 && password.length() > 0) 
        {
            Serial.println("Clearing SSID ...");
            for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
        }

        Serial.printf("Writing SSID (%s) ...\n", ssid.c_str());
        for (int i = 0; i < ssid.length(); ++i) EEPROM.write(i, ssid[i]);

        Serial.printf("Writing Password (%s)\n ...", password.c_str()); 
        for (int i = 0; i < password.length(); ++i) EEPROM.write(32 + i, password[i]);

        EEPROM.commit();
        ESP.restart();
    }
}