#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include "SIM7020.h"

#define DEFAULT_RESET_PIN 27
#define DEFAULT_POWERKEY_PIN 26
#define DEFAULT_RTCEINT_PIN 4
#define DEFAULT_BAUDRATE 115200

static String responseBufferstorage = "";
static SIM7020 *gmodem = NULL;
static GodmodeState *state = NULL;

unittest_setup()
{
    state = GODMODE();
    state->reset();

    gmodem = new SIM7020(&Serial, 27, 26, 4);
}

unittest_teardown()
{
    state->reset();
    delete gmodem;
    gmodem = NULL;
}

unittest(PinsAreLOW_AfterDefaultInitialization)
{
    /* Initialize mock objects */
    state->digitalPin[DEFAULT_RESET_PIN] = HIGH;
    state->digitalPin[DEFAULT_POWERKEY_PIN] = HIGH;
    state->digitalPin[DEFAULT_RTCEINT_PIN] = HIGH;

    /* Initialize module under test */

    SIM7020 *modem = new SIM7020(&Serial, 27, 26, 4);

    /* Assertions */
    assertEqual(LOW, state->digitalPin[DEFAULT_RESET_PIN]);
    assertEqual(LOW, state->digitalPin[DEFAULT_POWERKEY_PIN]);
    assertEqual(LOW, state->digitalPin[DEFAULT_RTCEINT_PIN]);

    delete modem;
}

unittest(OnlyResetPinsStateChangeToLow_AfterMinimumInitialization)
{
    /* Initialize mock objects */
    state->digitalPin[DEFAULT_RESET_PIN] = HIGH;
    state->digitalPin[DEFAULT_POWERKEY_PIN] = HIGH;
    state->digitalPin[DEFAULT_RTCEINT_PIN] = HIGH;

    SIM7020 *modem = new SIM7020(&Serial, 27);

    /* Assertions */
    assertEqual(LOW, state->digitalPin[DEFAULT_RESET_PIN]);
    assertEqual(HIGH, state->digitalPin[DEFAULT_POWERKEY_PIN]);
    assertEqual(HIGH, state->digitalPin[DEFAULT_RTCEINT_PIN]);

    delete modem;
}

unittest(ThisTestFails_WhenBaudrateDoesntMatchWithModem)
{
    gmodem->begin(115200);
}

unittest(BeginSucceeds_RestartEnabled)
{
    gmodem->begin(115200, true);

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
    gmodem->begin(115200, true);
    gmodem->sendATCommand("AT");

    assertEqual("AT\r", state->serialPort[0].dataOut);
}

unittest(ModemReturnsCorrectReponseStatuses)
{
    gmodem->begin(115200, true);
    gmodem->sendATCommand("AT\r");

    /* Command Success*/
    state->serialPort[0].dataIn = "\r\nOK\r\n";
    String responseBufferstorage;
    ATResponseStatus_t status = gmodem->waitForResponse(300, responseBufferstorage);
    assertEqual(CommandSuccess, status);
    assertEqual("\r\nOK\r\n", responseBufferstorage);
    /* Timeout Error occurs*/
    state->serialPort[0].dataIn = "";
    status = gmodem->waitForResponse(300, responseBufferstorage);
    assertEqual(TimeoutError, status);
    assertEqual("", responseBufferstorage);
    /* Invalid command error occurs*/
    state->serialPort[0].dataIn = "\r\nERROR\r\n";
    status = gmodem->waitForResponse(300, responseBufferstorage);
    assertEqual(InvalidCommand, status);
    assertEqual("\r\nERROR\r\n", responseBufferstorage);
    /* Unknown error due to potential hardware issue occurs*/
    state->serialPort[0].dataIn = "\r\n@3s#fsfs@\r\n";
    status = gmodem->waitForResponse(300, responseBufferstorage);
    assertEqual(UnknownStatus, status);
    assertEqual("\r\n@3s#fsfs@\r\n", responseBufferstorage);
}

unittest(ModemIsReady)
{
    state->serialPort[0].dataIn = "\r\nOK\r\n";
    assertEqual(true, gmodem->ready());
    assertEqual("AT\r", state->serialPort[0].dataOut);
}

unittest(ModemIsNotReady)
{
    state->serialPort[0].dataIn = "";
    assertEqual(false, gmodem->ready());
    assertEqual("AT\r", state->serialPort[0].dataOut);
}

unittest(PowerOnModemByHardwarePin)
{
    gmodem->turnPowerOn();
    bool actual[4];
    bool expected[4] = {LOW, LOW, LOW, HIGH};
    int numMoved = state->digitalPin[DEFAULT_POWERKEY_PIN].toArray(actual, 4);

    assertEqual(4, numMoved);
    for (auto i{0}; i < 4; i++)
    {
        assertEqual(expected[i], actual[i]);
    }
}

unittest(GetIMEI)
{
    state->serialPort[0].dataIn = "\r\n123456789012345\r\n\r\nOK\r\n";
    String imei;
    assertEqual("123456789012345", gmodem->getIMEI());
}

unittest_main()