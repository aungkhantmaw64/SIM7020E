#include "SIM7020.h"

SIM7020::SIM7020(HardwareSerial &serial, unsigned long baudrate, int resetPin)
{
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, LOW);
}

SIM7020::~SIM7020()
{
}