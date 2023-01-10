#include "TimeService.h"

#ifndef MOCK_PINS_COUNT

unsigned long getMillis(void)
{
    return millis();
}

#endif
