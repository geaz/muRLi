#ifdef BUILD_MURLI

#include "murli/murli_context.hpp"

Murli::MurliContext murliContext;

void setup()
{
    Serial.begin(74880);
    Serial.println("Starting muRLi...");
    Wire.begin();

    murliContext.setup();

    pinMode(A0, INPUT);                 // Mic Input
    pinMode(D6, INPUT);                 // Check if module inserted
    pinMode(Murli::LedDataPin, OUTPUT); // LED
}

void loop() 
{
    murliContext.loop();
}

#elif BUILD_MURLINODE

#endif