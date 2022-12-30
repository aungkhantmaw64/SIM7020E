#ifndef D_BC95_H
#define D_BC95_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <Client.h>

typedef enum ATResponseStatus
{
    UnknownStatus = -1,
    InvalidCommand = 0,
    TimeoutError,
    CommandSuccess,
} ATResponseStatus_t;

class SIM7020
{
private:
    HardwareSerial *serial_;
    int resetPin_;
    int pwrKeyPin_;
    int rtcEintPin_;
    String responseBuffer_;

public:
    /**
     * @brief Construct a new SIM7020 object
     *
     * @param serial
     * @param resetPin
     * @param pwrKeyPin
     * @param rtcEintPin
     */
    SIM7020(HardwareSerial *serial,
            int resetPin,
            int pwrKeyPin,
            int rtcEintPin);
    /**
     * @brief Construct a new SIM7020 object
     *
     * @param serial
     * @param resetPin
     */
    SIM7020(HardwareSerial *serial,
            int resetPin);
    /**
     * @brief Destroy the SIM7020 object
     *
     */
    ~SIM7020();
    /**
     * @brief
     *
     * @param baudrate
     */
    void begin(unsigned long baudrate);
    /**
     * @brief
     *
     * @param baudrate
     * @param restart
     */
    void begin(unsigned long baudrate, bool restart);
    /**
     * @brief
     *
     * @param cmd
     */
    void sendATCommand(const char *cmd);
    /**
     * @brief
     *
     */
    void hardReset(void);
    /**
     * @brief
     *
     * @return true
     * @return false
     */
    bool ready(void);
    /**
     * @brief
     *
     * @param timeout
     * @param responseBufferStorage
     * @return ATResponseStatus
     */
    ATResponseStatus waitForResponse(unsigned long timeout, String &responseBufferStorage);
    /**
     * @brief
     *
     */
    void end(void);
};

#endif