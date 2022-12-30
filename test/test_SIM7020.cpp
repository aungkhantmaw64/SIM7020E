#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include "SIM7020.h"

#define DEFAULT_RESET_PIN 27
#define DEFAULT_POWERKEY_PIN 26
#define DEFAULT_RTCEINT_PIN 4
#define DEFAULT_BAUDRATE 115200

unittest(PinsAreLOW_AfterDefaultInitialization)
{
    /* Initialize mock objects */
    GodmodeState *state = GODMODE();
    state->digitalPin[DEFAULT_RESET_PIN] = HIGH;
    state->digitalPin[DEFAULT_POWERKEY_PIN] = HIGH;
    state->digitalPin[DEFAULT_RTCEINT_PIN] = HIGH;

    /* Initialize module under test */

    SIM7020 *module = new SIM7020(&Serial, 27, 26, 4);

    /* Assertions */
    assertEqual(LOW, state->digitalPin[DEFAULT_RESET_PIN]);
    assertEqual(LOW, state->digitalPin[DEFAULT_POWERKEY_PIN]);
    assertEqual(LOW, state->digitalPin[DEFAULT_RTCEINT_PIN]);

    delete module;
}

unittest(OnlyResetPinsStateChangeToLow_AfterMinimumInitialization)
{
    /* Initialize mock objects */
    GodmodeState *state = GODMODE();
    state->digitalPin[DEFAULT_RESET_PIN] = HIGH;
    state->digitalPin[DEFAULT_POWERKEY_PIN] = HIGH;
    state->digitalPin[DEFAULT_RTCEINT_PIN] = HIGH;

    SIM7020 *module = new SIM7020(&Serial, 27);

    /* Assertions */
    assertEqual(LOW, state->digitalPin[DEFAULT_RESET_PIN]);
    assertEqual(HIGH, state->digitalPin[DEFAULT_POWERKEY_PIN]);
    assertEqual(HIGH, state->digitalPin[DEFAULT_RTCEINT_PIN]);

    delete module;
}

unittest(ThisTestFails_WhenBaudrateDoesntMatchWithModem)
{
    SIM7020 *module = new SIM7020(&Serial, 27);
    module->begin(115200);
    delete module;
}

unittest(BeginSucceeds_RestartEnabled)
{
    GodmodeState *state = GODMODE();
    state->reset();

    SIM7020 *module = new SIM7020(&Serial, 27);
    module->begin(115200, true);
    delete module;

    bool actual[4];
    bool expected[4] = {LOW, LOW, HIGH, LOW};
    int numMoved = state->digitalPin[DEFAULT_RESET_PIN].toArray(actual, 4);
    assertEqual(4, numMoved);

    for (int i = 0; i < 2; i++)
    {
        assertEqual(expected[i], actual[i]);
    }
}

unittest(ATCommandSends)
{
    GodmodeState *state = GODMODE();
    state->reset();

    SIM7020 *module = new SIM7020(&Serial, 27);
    module->begin(115200, true);
    module->sendATCommand("AT\r");
    delete module;

    assertEqual("AT\r", state->serialPort[0].dataOut);
}

unittest_main()