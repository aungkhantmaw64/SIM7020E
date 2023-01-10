#include <Arduino.h>
#include "SIM7020.h"

#define RESET_PIN 27
#define POWERKEY_PIN 26
#define RTCEINT_PIN 4
#define BAUDRATE 115200

SIM7020 module(Serial2, RESET_PIN, POWERKEY_PIN, RTCEINT_PIN);

void setup()
{
    Serial.begin(115200);
    Serial.println("Rebooting...");
    Serial2.begin(115200);
    String response;
    response.reserve(100);
    module.begin(false);
    /*Sending AT commands*/
    module.sendATCommand("AT+GSN");
    int ret = module.waitForResponse(1000, response);
    Serial.print("Code: ");
    Serial.println(ret);
    Serial.print("Response: ");
    Serial.println(response);

    /* Modem APIs */
    while (!module.ready())
    {
        Serial.print(".");
    }
    Serial.println("Modem Information");
    Serial.println("-----------------");
    String imei = module.getIMEI();
    Serial.println("IMEI: " + imei);
}

void loop()
{
}