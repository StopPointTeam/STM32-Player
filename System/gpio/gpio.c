//管理所有 GPIO 的初始化

#include "sys.h"

#include "gpio.h"

/**************************************** GPIO A1 ****************************************/

/**
  * @brief  PA1 初始化，用于 LED2
  */
void GPIO_A1_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE(); //开启 PA 时钟

    GPIO_Initure.Pin = GPIO_PIN_1;                  //PA1
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;        //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;                //上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; //高速
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);            //初始化 PA1
}

/****************************************************************************************/

/************************************** GPIO E3 C5 **************************************/

/**
  * @brief  PE3 PC5 初始化，用于 KEY1 KEY2
  */
void GPIO_E3_C5_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE(); //开启 PC 时钟
    __HAL_RCC_GPIOE_CLK_ENABLE(); //开启 PE 时钟

    GPIO_Initure.Pin = GPIO_PIN_3;                  //PE3：对应 K1
    GPIO_Initure.Mode = GPIO_MODE_INPUT;            //输入
    GPIO_Initure.Pull = GPIO_PULLUP;                //上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; //高速
    HAL_GPIO_Init(GPIOE, &GPIO_Initure);            //初始化 PE3

    GPIO_Initure.Pin = GPIO_PIN_5;                  //PC5：对应 K2
    GPIO_Initure.Mode = GPIO_MODE_INPUT;            //输入
    GPIO_Initure.Pull = GPIO_PULLUP;                //上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; //高速
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);            //初始化 PC5
}

/****************************************************************************************/

/************************************ GPIO B0 B1 B12 ************************************/

/**
  * @brief  PB0 PB1 PB12 初始化，用于 LCD 的 BLK RS CS
  */
void GPIO_B0_B1_B12_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE(); //开启 PB 时钟

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_12; //PB0 PB1 PB12
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;                  //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;                          //上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;           //高速
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);                      //初始化 PB0 PB1 PB12
}

/****************************************************************************************/
