#ifndef D_BC95_H
#define D_BC95_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <Client.h>

class SIM7020
{
public:
    SIM7020(HardwareSerial &serial, unsigned long baudrate, int resetPin);
    ~SIM7020();
};

#endif