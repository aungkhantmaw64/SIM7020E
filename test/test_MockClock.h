#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;

namespace MockClockTest
{
    class MockClock
    {
    private:
        unsigned long startTime;
        unsigned long tick;

    public:
        MockClock(unsigned long startTime, unsigned long tick)
        {
            this->startTime = startTime;
            this->tick = tick;
        }
        ~MockClock()
        {
            startTime = 0;
            tick = 0;
        }
        void start(void)
        {
            When(Method(ArduinoFake(), millis))
                .AlwaysDo([this]() -> unsigned long
                          {
            unsigned long time = startTime;
            startTime += tick;
            return time; });
        }
    };

    void test_MockClock(void)
    {
        MockClock *mock_clock = new MockClock(0, 10);
        mock_clock->start();

        TEST_ASSERT_EQUAL(0, millis());
        TEST_ASSERT_EQUAL(10, millis());
        delete mock_clock;
    }

    void run_tests(void)
    {
        RUN_TEST(test_MockClock);
    }
}
