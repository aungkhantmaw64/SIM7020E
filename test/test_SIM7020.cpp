#include <ArduinoFake.h>
#include <unity.h>
#include "SIM7020.h"
#include <cstring>

#define RESET_PIN 27
#define POWERKEY_PIN 26
#define RTCEINT_PIN 4
#define BAUDRATE 115200

using namespace fakeit;

static SIM7020 *underTest = NULL;
Stream *serial = NULL;

void setUp(void)
{
    serial = (StreamFakeProxy *)ArduinoFakeMock(Serial);
    underTest = new SIM7020(serial, RESET_PIN, POWERKEY_PIN, RTCEINT_PIN);
}

void tearDown(void)
{
    delete underTest;
    ArduinoFakeReset();
}

void test_BeginSucceeds_WithoutRestart(void)
{
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), millis)).Return(1000, 2000, 2025, 4025);

    underTest->begin(false);

    /* Test pin intialization case */
    Verify(Method(ArduinoFake(), pinMode).Using(RESET_PIN, OUTPUT),
           Method(ArduinoFake(), pinMode).Using(POWERKEY_PIN, OUTPUT),
           Method(ArduinoFake(), pinMode).Using(RTCEINT_PIN, OUTPUT));
    Verify(Method(ArduinoFake(), digitalWrite).Using(POWERKEY_PIN, LOW),
           Method(ArduinoFake(), digitalWrite).Using(POWERKEY_PIN, HIGH));
    Verify(Method(ArduinoFake(), millis)).Exactly(4);
}

void test_SendATCommandsAppendCarriageReturn(void)
{
    When(Method(ArduinoFake(), millis)).Return(0, 20);
    When(OverloadedMethod(ArduinoFake(Serial), print, size_t(char))).AlwaysReturn();
    When(Method(ArduinoFake(Serial), flush)).Return();

    underTest->sendATCommand("AT");

    Verify(Method(ArduinoFake(), millis)).Exactly(2);
    char cmd[] = "AT\r";
    for (int i{0}; i < strlen(cmd); i++)
    {
        Verify(OverloadedMethod(ArduinoFake(Serial), print, size_t(char))
                   .Using(cmd[i]))
            .Once();
    }

    Verify(Method(ArduinoFake(Serial), flush)).Once();
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_BeginSucceeds_WithoutRestart);
    RUN_TEST(test_SendATCommandsAppendCarriageReturn);
    UNITY_END();
}