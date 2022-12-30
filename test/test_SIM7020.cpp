#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include "SIM7020.h"

unittest_setup()
{
}

unittest_teardown()
{
}

unittest(InitalizationDoesNotResetModem)
{
    GodmodeState *state = GODMODE();
    state->digitalPin[18] = HIGH;

    SIM7020(Serial1, 9600, 18);
    assertEqual(LOW, state->digitalPin[18]);
}

unittest_main()