#include "sys.h"
#include "systick.h"

#include "key.h"

#include "GameEngine.h"
#include "WLAN.h"

#include "APP_Weather.h"

/**************************************** ˽�ж��� ****************************************/

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

/**************************************** ȫ�ֱ��� ****************************************/

Weather weather;
uint8_t city_str[30];

/*****************************************************************************************/

/**************************************** ˽�к��� ****************************************/

void APP_Weather_SetCity(uint8_t *str);
uint8_t APP_Weather_Sync(void);

/*****************************************************************************************/

/**
  * @brief  ��������Ӧ��
  */
void APP_Weather_Launcher(void)
{
    APP_Weather_SetCity("����");

    GE_Draw_ClrAll(WHITE);

    if (APP_Weather_Sync())
    {
        printf("������ȡ�����ɹ�\n����: %s %s %s %s %s %s %s\n",
               weather.city,
               weather.weather,
               weather.temp,
               weather.wind_dir,
               weather.wind_force,
               weather.humidity,
               weather.quality);

        uint8_t temp_str[200];
        sprintf(temp_str,
                "���У�%s\n������%s\n�¶ȣ�%s\n����%s\n������%s\nʪ�ȣ�%s\n����������%s\n",
                weather.city,
                weather.weather,
                weather.temp,
                weather.wind_dir,
                weather.wind_force,
                weather.humidity,
                weather.quality);
        GE_GUI_MsgBox(5, 5, 310, 230, "����", temp_str, NULL);
    }
    else
    {
        printf("������ȡ����ʧ��\n");

        GE_GUI_MsgBox(5, 5, 310, 230, "����", "������ȡ����ʧ��", NULL); 
    }

    KEY_WaitKey(JOY_L);
}

/**
  * @brief  ѡ�����
  * @param  str: ������
  */
void APP_Weather_SetCity(uint8_t *str)
{
    strcpy(city_str, str);
}

/**
  * @brief  ͬ������
  * @retval �ɹ����� 1��ʧ�ܷ��� 0
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
