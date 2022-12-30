#include "SIM7020.h"

#define __ASSERT_USE_STDERR

#include <assert.h>
#include "TimeService.h"

#define SIM7020_DEFAULT_BAUD 115200

static inline void setPinActiveLow(int pin)
{
    if (pin >= 0)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
}

static inline void delay_ms(unsigned long milliseconds)
{
    unsigned long startTime = getMillis();
    while ((getMillis() - startTime) < 48)
        ;
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

void SIM7020::begin(unsigned long baudrate, bool restart)
{
    begin(baudrate);
    if (restart)
    {
        hardReset();
    }
}

void SIM7020::hardReset(void)
{
    digitalWrite(resetPin_, HIGH);
    delay_ms(48);
    digitalWrite(resetPin_, LOW);
}

void SIM7020::sendATCommand(const char *cmd)
{
    delay_ms(20);
    serial_->print(cmd);
}

ATResponseStatus SIM7020::waitForResponse(unsigned long timeout, String &responseBufferStorage)
{
    responseBufferStorage = "";
    unsigned long startTime = getMillis();
    while ((getMillis() - startTime) < timeout)
    {
        if (serial_->available())
        {
            responseBufferStorage += (char)serial_->read();
        }
    }
    if (responseBufferStorage.length() == 0)
        return TimeoutError;
    else if (responseBufferStorage.indexOf("OK") != -1)
        return CommandSuccess;
    else if (responseBufferStorage.indexOf("ERROR") != -1)
        return InvalidCommand;
    else
        return UnknownStatus;
}

bool SIM7020::ready(void)
{
    sendATCommand("AT\r");
    return (CommandSuccess == waitForResponse(300, responseBuffer_));
}

SIM7020::~SIM7020()
{
}