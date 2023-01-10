#include "SIM7020.h"

#define __ASSERT_USE_STDERR

#include <assert.h>
#include "TimeService.h"

#define SIM7020_DEFAULT_BAUD 115200
#define TYPICAL_IMEI_LENGTH 15

static inline void delay_ms(unsigned long milliseconds)
{
    unsigned long startTime = getMillis();
    while ((getMillis() - startTime) < milliseconds)
        ;
}

SIM7020::SIM7020(HardwareSerial &serial, int resetPin, int pwrKeyPin, int rtcEintPin)
    : serial_(&serial),
      resetPin_(resetPin),
      pwrKeyPin_(pwrKeyPin),
      rtcEintPin_(rtcEintPin)
{
    pinMode(pwrKeyPin_, OUTPUT);
}

SIM7020::SIM7020(HardwareSerial &serial, int resetPin)
    : SIM7020(serial, resetPin, -1, -1)
{
}

void SIM7020::begin(bool restart)
{
    if (restart)
    {
        hardReset();
    }
    serial_->begin(115200);
    turnPowerOn();
    sendATCommand("ATE0");
    String response;
    waitForResponse(300, response);
}

void SIM7020::turnPowerOn(void)
{
    digitalWrite(pwrKeyPin_, LOW);
    delay_ms(1000);
    digitalWrite(pwrKeyPin_, HIGH);
    delay_ms(2000);
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
    serial_->print("\r");
    serial_->flush();
}

ATResponseStatus SIM7020::waitForResponse(unsigned long timeout, String &responseBufferStorage)
{
    responseBufferStorage = "";
    unsigned long startTime = getMillis();
    while ((getMillis() - startTime) < timeout)
    {
        if (serial_->available())
        {
            char ch = (char)serial_->read();
            responseBufferStorage += ch;
        }
    }
    if (responseBufferStorage.length() == 0)
        return TimeoutError;
    else if (responseBufferStorage.lastIndexOf("OK\r\n") != -1)
        return CommandSuccess;
    else if (responseBufferStorage.lastIndexOf("ERROR\r\n") != -1)
        return InvalidCommand;
    else
        return UnknownStatus;
}

bool SIM7020::ready(void)
{
    String response;
    response.reserve(20);
    sendATCommand("AT");
    return (CommandSuccess == waitForResponse(300, response));
}

String SIM7020::getIMEI(void)
{
    String imei;
    imei.reserve(100);
    sendATCommand("AT+GSN");
    int ret = waitForResponse(300, imei);
    if (ret == CommandSuccess)
    {
        imei.trim();
        return imei.substring(0, TYPICAL_IMEI_LENGTH);
    }
    return "";
}

SIM7020::~SIM7020()
{
}