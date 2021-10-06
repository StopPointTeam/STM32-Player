#include "sys.h"
#include "systick.h"

#include "key.h"

#include "GameEngine.h"
#include "WLAN.h"

#include "APP_Weather.h"

/**************************************** 私有定义 ****************************************/

typedef struct
{
    uint8_t city[20];
    uint8_t weather[20];
    uint8_t temp[20];
    uint8_t wind_dir[20];
    uint8_t wind_force[20];
    uint8_t humidity[20];
    uint8_t quality[20];
} Weather;

/*****************************************************************************************/

/**************************************** 全局变量 ****************************************/

Weather weather;
uint8_t city_str[30];

/*****************************************************************************************/

/**************************************** 私有函数 ****************************************/

void APP_Weather_SetCity(uint8_t *str);
uint8_t APP_Weather_Sync(void);

/*****************************************************************************************/

/**
  * @brief  启动天气应用
  */
void APP_Weather_Launcher(void)
{
    APP_Weather_SetCity("杭州");

    GE_Draw_ClrAll(WHITE);

    if (APP_Weather_Sync())
    {
        printf("连网获取天气成功\n天气: %s %s %s %s %s %s %s\n",
               weather.city,
               weather.weather,
               weather.temp,
               weather.wind_dir,
               weather.wind_force,
               weather.humidity,
               weather.quality);

        uint8_t temp_str[200];
        sprintf(temp_str,
                "城市：%s\n天气：%s\n温度：%s\n风向：%s\n风力：%s\n湿度：%s\n空气质量：%s\n",
                weather.city,
                weather.weather,
                weather.temp,
                weather.wind_dir,
                weather.wind_force,
                weather.humidity,
                weather.quality);
        GE_GUI_MsgBox(5, 5, 310, 230, "天气", temp_str, NULL);
    }
    else
    {
        printf("连网获取天气失败\n");

        GE_GUI_MsgBox(5, 5, 310, 230, "天气", "连网获取天气失败", NULL); 
    }

    KEY_WaitKey(JOY_L);
}

/**
  * @brief  选择城市
  * @param  str: 城市名
  */
void APP_Weather_SetCity(uint8_t *str)
{
    strcpy(city_str, str);
}

/**
  * @brief  同步天气
  * @retval 成功返回 1，失败返回 0
  */
uint8_t APP_Weather_Sync(void)
{
    uint8_t weather_str[50];
    if (WLAN_GetWeather(weather_str, city_str) != 1)
        return 0;

    uint8_t *p;

    strtok(weather_str, "&");
    strcpy(weather.city, weather_str);
    p = strtok(NULL, "&");
    strcpy(weather.weather, p);
    p = strtok(NULL, "&");
    strcpy(weather.temp, p);
    p = strtok(NULL, "&");
    strcpy(weather.wind_dir, p);
    p = strtok(NULL, "&");
    strcpy(weather.wind_force, p);
    p = strtok(NULL, "&");
    strcpy(weather.humidity, p);
    p = strtok(NULL, "&");
    strcpy(weather.quality, p);

    return 1;
}
