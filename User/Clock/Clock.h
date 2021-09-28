#ifndef __CLOCK_H
#define __CLOCK_H

#include "sys.h"

typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} ClockTime;

void Clock_Init(void);
uint8_t Clock_Sync(void);

void Clock_Handler(void);

#endif
