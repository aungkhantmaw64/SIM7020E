#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include "SIM7020.h"

#define DEFAULT_RESET_PIN 27
#define DEFAULT_POWERKEY_PIN 26
#define DEFAULT_RTCEINT_PIN 4
#define DEFAULT_BAUDRATE 115200

static GodmodeState *state = NULL;
static SIM7020 *module = new SIM7020(&Serial1,
                                     DEFAULT_RESET_PIN,
                                     DEFAULT_POWERKEY_PIN,
                                     DEFAULT_RTCEINT_PIN);

unittest_setup()
{
    /* Mock object intialization */
    state = GODMODE();
    state->reset();
    state->resetClock();
    state->resetPins();

    state->digitalPin[DEFAULT_RESET_PIN] = HIGH;
    state->digitalPin[DEFAULT_POWERKEY_PIN] = HIGH;
    state->digitalPin[DEFAULT_RTCEINT_PIN] = HIGH;
}

unittest_teardown()
{
}

unittest(ConstructorInitalization_PutAllTheModemPinsToLowState)
{
    int resetPin = 27;
    int pwrKeyPin = 26;
    int rtcEintPin = 4;

    SIM7020(&Serial1,
            resetPin,
            pwrKeyPin,
            rtcEintPin);

    assertEqual(LOW, state->digitalPin[DEFAULT_RESET_PIN]);
    assertEqual(LOW, state->digitalPin[DEFAULT_POWERKEY_PIN]);
    assertEqual(LOW, state->digitalPin[DEFAULT_RTCEINT_PIN]);
}

unittest(MinimumInitalizationSucceeds_PutResetPinToLowState)
{
    int resetPin = 27;
    SIM7020(&Serial1,
            resetPin);

    assertEqual(LOW, state->digitalPin[DEFAULT_RESET_PIN]);
}

unittest(BeginSucceeds_BaudrateMatchWithModem)
{
    module->begin(115200);
}

unittest_main()