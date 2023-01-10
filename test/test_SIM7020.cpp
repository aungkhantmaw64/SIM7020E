#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include "SIM7020.h"

#define DEFAULT_RESET_PIN (int)(27)
#define DEFAULT_POWERKEY_PIN (int)(26)
#define DEFAULT_RTCEINT_PIN (int)(4)
#define DEFAULT_BAUDRATE (unsigned long)(115200)

class MockSerial : public Stream
{
private:
    unsigned long baudrate;

public:
    MockSerial()
    {
    }
    ~MockSerial()
    {
    }
    unsigned long getBaudrate(void)
    {
        return baudrate;
    }
};

static MockSerial mock_serial;

static SIM7020 modem(mock_serial,
                     DEFAULT_RESET_PIN,
                     DEFAULT_POWERKEY_PIN,
                     DEFAULT_RTCEINT_PIN);

static GodmodeState *state = NULL;

unittest_setup()
{
    state = GODMODE();
    state->reset();
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

unittest_main()