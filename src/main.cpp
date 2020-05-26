#ifdef BUILD_MURLICONSOLE

#include "led/led.hpp"
#include "wifi/murli_wifi.hpp"
#include "display/display.hpp"
#include "display/splashView.cpp"
#include "murli/murli_context.hpp"

Murli::LED led;
Murli::MurliWifi wifi;

Murli::Display display;
Murli::SplashView splashView;

Murli::MurliContext murliContext(display, led);

void setup()
{
    Serial.begin(74880);
    Serial.println("Starting muRLi...");
    Wire.begin();

    pinMode(A0, INPUT);                 // Mic Input
    pinMode(D6, INPUT);                 // Check if module inserted
    pinMode(Murli::LedDataPin, OUTPUT); // LED

    display.init();
    display.setView(std::make_shared<Murli::SplashView>());
    display.setLeftStatus("Starting Mesh...");
    display.loop();
    
    wifi.startMesh();
    
    display.setLeftStatus("");
    display.setRightStatus("");
}

void loop() 
{
    murliContext.loop();
    display.loop();
}

#elif BUILD_MURLINODE

#endif