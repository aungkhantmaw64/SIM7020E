#include <Arduino.h>
#include <cstdlib>
#include <unity.h>

#include "test_SIM7020.h"
#include "test_MockClock.h"
#include "test_MockSerial.h"

using namespace fakeit;

#ifdef UNIT_TEST

#define RUN_TEST_GROUP(TEST)                                                           \
    if (!std::getenv("TEST_GROUP") || (strcmp(#TEST, std::getenv("TEST_GROUP")) == 0)) \
    {                                                                                  \
        TEST::run_tests();                                                             \
    }

void setUp(void)
{
}

void tearDown(void)
{
    ArduinoFakeReset();
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST_GROUP(MockClockTest);
    RUN_TEST_GROUP(MockSerialTest);
    RUN_TEST_GROUP(Sim7020Test);
    UNITY_END();

    return 0;
}

#endif