#ifndef D_SIM7020_H
#define D_SIM7020_H

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
    Stream *serial_;
    int resetPin_;
    int pwrKeyPin_;
    int rtcEintPin_;
    String resBuffer_;

public:
    /**
     * @brief Construct a new SIM7020 object
     *
     * @param serial
     * @param resetPin
     * @param pwrKeyPin
     * @param rtcEintPin
     */
    SIM7020(Stream &serial,
            int resetPin,
            int pwrKeyPin,
            int rtcEintPin);
    /**
     * @brief Construct a new SIM7020 object
     *
     * @param serial
     * @param resetPin
     */
    SIM7020(Stream &serial,
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
     * @param restart
     */
    void begin(bool restart);
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
     */
    void turnPowerOn(void);
    /**
     * @brief
     *
     */
    void turnPowerOff(void);
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
     * @return String
     */
    String getIMEI(void);
    /**
     * @brief
     *
     * @return String
     */
    String getICCID(void);
    /**
     * @brief
     *
     */
    void end(void);
};

#endif