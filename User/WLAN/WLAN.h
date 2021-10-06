//WLAN ¿â

#ifndef __WLAN_H
#define __WLAN_H

#include "sys.h"

#include "Clock.h"

uint8_t WLAN_CheckNet(void);
uint8_t WLAN_GetIPAddr(uint8_t *ipaddr_str);
uint8_t WLAN_GetNetClockTime(uint8_t *clock_time_str);
uint8_t WLAN_GetWeather(uint8_t *weather_str, uint8_t *city_str);

#endif
