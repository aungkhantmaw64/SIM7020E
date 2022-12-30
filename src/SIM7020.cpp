#include "SIM7020.h"

#define __ASSERT_USE_STDERR

#include <assert.h>

#define SIM7020_DEFAULT_BAUD 115200

static inline void setPinActiveLow(int pin)
{
    if (pin >= 0)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
}

SIM7020::SIM7020(HardwareSerial *serial, int resetPin, int pwrKeyPin, int rtcEintPin)
    : serial_(serial),
      resetPin_(resetPin),
      pwrKeyPin_(pwrKeyPin),
      rtcEintPin_(rtcEintPin)
{
    setPinActiveLow(resetPin_);
    setPinActiveLow(pwrKeyPin_);
    setPinActiveLow(rtcEintPin_);
}

SIM7020::SIM7020(HardwareSerial *serial, int resetPin)
    : SIM7020(serial, resetPin, -1, -1)
{
}

void SIM7020::begin(unsigned long baudrate)
{
    assert((baudrate == SIM7020_DEFAULT_BAUD) && "Baudrate doesn't match with Modem");
    serial_->begin(baudrate);
}

SIM7020::~SIM7020()
{
}