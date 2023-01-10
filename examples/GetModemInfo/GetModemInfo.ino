#include <Arduino.h>
#include "SIM7020.h"

#define RESET_PIN 27
#define POWERKEY_PIN 26
#define RTCEINT_PIN 4
#define BAUDRATE 115200

SIM7020 module(&Serial2, RESET_PIN, POWERKEY_PIN, RTCEINT_PIN);

void setup()
{
    Serial.begin(115200);
    module.begin(BAUDRATE, true);
    while (!module.ready())
    {
        Serial.print(".");
    }
    Serial.println();
    Serial.println("IMEI: " + module.getIMEI());
}

void loop()
{
}