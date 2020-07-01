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

#include "murli/murliN_context.hpp"

Murli::MurlinContext murlinContext;

void setup()
{
    Serial.begin(74880);
    Serial.println("Starting muRLiN ...");

    murlinContext.setup();

    pinMode(LED_PIN, OUTPUT);    
}

void loop()
{
    murlinContext.loop();
}

#endif