#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include "SIM7020.h"
#include "TimeService.h"

#define DEFAULT_RESET_PIN (int)(27)
#define DEFAULT_POWERKEY_PIN (int)(26)
#define DEFAULT_RTCEINT_PIN (int)(4)
#define DEFAULT_BAUDRATE (unsigned long)(115200)

static unsigned long start = 0;
static unsigned long step = 1;

#ifdef MOCK_PINS_COUNT
unsigned long getMillis(void)
{
    if (start < 10000UL)
        start += step;
    else
        start = 0;
    return start;
}
#endif

static SIM7020 modem(Serial,
                     DEFAULT_RESET_PIN,
                     DEFAULT_POWERKEY_PIN,
                     DEFAULT_RTCEINT_PIN);

static GodmodeState *state = NULL;

unittest_setup()
{
    state = GODMODE();
    state->reset();
    state->resetClock();
}

unittest_teardown()
{
    state->reset();
}

unittest(BeginTurnsModemPowerOn_IgnoreRestart)
{
    modem.begin(false);

    /*Text fixtures*/
    const int powerKeyPin = 26;
    int totalExpectedPinStateChanges = 3;
    bool initialState = LOW;
    bool expectedPinStates[3] = {initialState, LOW, HIGH};
    bool actualPinStates[3];
    int numStateChanges = state->digitalPin[powerKeyPin].toArray(actualPinStates,
                                                                 totalExpectedPinStateChanges);
    assertEqual(numStateChanges, totalExpectedPinStateChanges);
    for (auto index{0}; index < totalExpectedPinStateChanges; index++)
    {
        assertEqual(expectedPinStates[index], actualPinStates[index]);
    }
}

unittest(SendATCommand_CarriageReturnAppendedAutomatically)
{
    modem.sendATCommand("AT");

    assertEqual(state->serialPort[0].dataOut, "AT\r");
}

unittest(CommandSentSuccesfully_ModemRepondSuccessCode)
{
    state->serialPort[0].dataIn = "\r\nOK\r\n";

    String response;
    int retCode = modem.waitForResponse(300, response);

    assertEqual(CommandSuccess, retCode);
    assertEqual("\r\nOK\r\n", response);
}

unittest_main()