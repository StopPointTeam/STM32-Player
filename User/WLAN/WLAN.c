//WLAN ¿â

#include "stdio.h"
#include "string.h"

#include "systick.h"

#include "led.h"
#include "hc25.h"

#include "Clock.h"

#include "WLAN.h"

uint8_t WLAN_CheckNet(void)
{
    uint8_t str[6];

    HC25_ClearReceive;
    HC25_SendBuff("checknet", 9);

    str[0] = '\0';
    if (HC25_ReceiveBuffUntil(str, '\n', 1000) && strcmp(str, "OK") == 0)
        return 1;
    else
        return 0;
}

uint8_t WLAN_GetIPAddr(uint8_t *ipaddr_str)
{
    HC25_ClearReceive;
    HC25_SendBuff("ipaddr", 7);

    ipaddr_str[0] = '\0';
    if (HC25_ReceiveBuffUntil(ipaddr_str, '\n', 1000) && strcmp(ipaddr_str, "OK") == 0)
        if (HC25_ReceiveBuffUntil(ipaddr_str, '\n', 1000))
            return 1;

    return 0;
}

uint8_t WLAN_GetNetClockTime(uint8_t *clock_time_str)
{
    HC25_ClearReceive;
    HC25_SendBuff("time", 5);

    clock_time_str[0] = '\0';

    if (HC25_ReceiveBuffUntil(clock_time_str, '\n', 1000) && strcmp(clock_time_str, "OK") == 0)
        if (HC25_ReceiveBuffUntil(clock_time_str, '\n', 1000))
            return 1;

    return 0;
}

uint8_t WLAN_GetWeather(uint8_t *weather_str, uint8_t *city_str)
{
    uint8_t temp_str[15] = "weather=";

    HC25_ClearReceive;
    strcat(temp_str, city_str);
    HC25_SendBuff(temp_str, strlen(temp_str) + 1);

    weather_str[0] = '\0';

    if (HC25_ReceiveBuffUntil(weather_str, '\n', 1000) && strcmp(weather_str, "OK") == 0)
        if (HC25_ReceiveBuffUntil(weather_str, '\n', 1000))
            return 1;

    return 0;
}
