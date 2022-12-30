#include "TimeService.h"

static unsigned long start = 0;
static unsigned long step = 1;

#ifndef MOCK_PINS_COUNT

unsigned long getMillis(void)
{
    return millis();
}

#else

unsigned long getMillis(void)
{
    if (start < 10000UL)
        start += step;
    else
        start = 0;
    return start;
}

#endif