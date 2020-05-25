#ifdef BUILD_MURLICONSOLE

#include "led/led.hpp"
#include "led/color.hpp"
#include "wifi/murli_wifi.hpp"
#include "display/display.hpp"
#include "display/splashView.cpp"
#include "states/state.hpp"
#include "states/no_mod_state.cpp"
#include "states/receive_write_state.cpp"

Murli::LED led;
Murli::MurliWifi wifi;

Murli::Display display;
Murli::SplashView splashView;

Murli::State* currentState;
Murli::StateContext context;
Murli::NoModState noModState(led, display);
Murli::ReceiveWriteState receiveWriteState(led, display);

void setup() 
{
    Serial.begin(74880);
    Serial.println("Starting muRLi ...");
    Wire.begin();

    pinMode(A0, INPUT);                 // Mic Input
    pinMode(D6, INPUT);                 // Check if module inserted
    pinMode(Murli::LedDataPin, OUTPUT); // LED

    display.init();
    display.setView(&splashView);
    display.drawView();

    currentState = &noModState;
}

void loop() 
{
    // Check if there arrived a new write request
    if(Serial.available() > 0
        && !context.writeRequested 
        && currentState->isModInserted())
    {
        currentState = &receiveWriteState;
    }
    // Reset state, if no MOD inserted
    else if(!currentState->isModInserted())
    {
        currentState = &noModState;
    }        

    // Run current state
    currentState = currentState->run(context);
    display.drawView();
}

#elif BUILD_MURLINODE

#endif