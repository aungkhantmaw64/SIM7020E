#ifndef D_BC95_H
#define D_BC95_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <Client.h>

class SIM7020
{
private:
    HardwareSerial *serial_;
    int resetPin_;
    int pwrKeyPin_;
    int rtcEintPin_;

public:
    SIM7020(HardwareSerial *serial,
            int resetPin,
            int pwrKeyPin,
            int rtcEintPin);
    SIM7020(HardwareSerial *serial,
            int resetPin);
    ~SIM7020();
    void begin(unsigned long baudrate);
    void begin(unsigned long baudrate, bool restart);
    void end(void);
};

#endif