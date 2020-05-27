#ifdef BUILD_MURLICONSOLE

#include "murli/murli_context.hpp"

Murli::MurliContext murliContext;

void setup()
{
    Serial.begin(74880);
    Serial.println("Starting muRLi...");
    Wire.begin();

    pinMode(A0, INPUT);                 // Mic Input
    pinMode(D6, INPUT);                 // Check if module inserted
    pinMode(Murli::LedDataPin, OUTPUT); // LED

    murliContext.setup();
}

void loop() 
{
    murliContext.loop();
}

#elif BUILD_MURLINODE

#endif