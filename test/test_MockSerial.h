#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;

namespace MockSerialTest
{
    void test_MockSerial(void)
    {
        String buffer = "";
        unsigned long start = 0;
        unsigned long step = 10;
        const char response[] = "OK";
        int rindex = 0;
        int response_length = strlen(response);

        When(Method(ArduinoFake(), millis))
            .AlwaysDo([&start, &step]() -> unsigned long
                      {
            unsigned long time = start;
            start += step;
            return time; });

        When(Method(ArduinoFake(Stream), read))
            .AlwaysDo([response, &rindex, &response_length]() -> int
                      {
            if (response_length > 0){
                response_length--;
                return response[rindex++];
            }
            else{
                return -1; 
            } });

        When(Method(ArduinoFake(Stream), available))
            .AlwaysDo([&response_length]() -> int
                      { return response_length; });

        Stream *stream = ArduinoFakeMock(Stream);

        TEST_ASSERT_EQUAL(0, millis());
        TEST_ASSERT_EQUAL(10, millis());
        TEST_ASSERT_EQUAL(2, stream->available());
        TEST_ASSERT_EQUAL_CHAR('O', stream->read());
        TEST_ASSERT_EQUAL(1, stream->available());
        TEST_ASSERT_EQUAL('K', stream->read());
        TEST_ASSERT_EQUAL(0, stream->available());
    }

    void run_tests(void)
    {
        RUN_TEST(test_MockSerial);
    }
}