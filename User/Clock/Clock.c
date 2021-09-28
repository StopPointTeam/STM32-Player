#include "GameEngine.h"
#include "WLAN.h"

#include "Clock.h"

ClockTime clock_time = {0, 0, 0, 0, 0, 0};
uint8_t is_disp = 0;
uint16_t disp_x;
uint16_t disp_y;
uint8_t disp_mode;

void Clock_Init(void)
{
    printf(
        "连网获取时间是否成功: %d\n时间: %d-%d-%d %d:%d:%d\n",
        Clock_Sync(),
        clock_time.year,
        clock_time.month,
        clock_time.day,
        clock_time.hour,
        clock_time.minute,
        clock_time.second);
}

uint8_t Clock_Sync(void)
{
    uint8_t clock_time_str[20];
    if (WLAN_GetNetClockTime(clock_time_str) != 1)
        return 0;

    uint8_t *p;

    strtok(clock_time_str, "&");
    sscanf(clock_time_str, "%d", &clock_time.year);
    p = strtok(NULL, "&");
    sscanf(p, "%d", &clock_time.month);
    p = strtok(NULL, "&");
    sscanf(p, "%d", &clock_time.day);
    p = strtok(NULL, "&");
    sscanf(p, "%d", &clock_time.hour);
    p = strtok(NULL, "&");
    sscanf(p, "%d", &clock_time.minute);
    p = strtok(NULL, "&");
    sscanf(p, "%d", &clock_time.second);

    return 1;
}

void Clock_Handler(void)
{
    clock_time.second++;

    if (clock_time.second == 60)
    {
        clock_time.second = 0;
        clock_time.minute++;

        if (clock_time.minute == 60)
        {
            clock_time.minute = 0;
            clock_time.hour++;

            if (clock_time.hour == 24)
            {
                clock_time.hour = 0;
                clock_time.day++;
            }
        }
    }

    // static char temp_str[20];
    // if (is_disp)
    // {
    //     sprintf(
    //         temp_str,
    //         "%04d %02d %02d %02d %02d %02d",
    //         clock_time.year,
    //         clock_time.month,
    //         clock_time.day,
    //         clock_time.hour,
    //         clock_time.minute,
    //         clock_time.second);
    //     GE_Font_Print(disp_x, disp_y, BORDER_MAX, BORDER_MAX, FONT_16, BLUE, WHITE, FALSE, temp_str);
    //     GE_Draw_Disp();
    // }
}
