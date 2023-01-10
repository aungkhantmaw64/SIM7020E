#include "TimeService.h"

#ifndef MOCK_PINS_COUNT

unsigned long getMillis(void)
{
    return millis();
}

#else

static unsigned long start = 0;
static unsigned long step = 1;

unsigned long getMillis(void)
{
    if (start < 10000UL)
        start += step;
    else
        start = 0;
    return start;
}

#endif