//�������� GPIO �ĳ�ʼ��

#include "sys.h"

#include "gpio.h"

/**************************************** GPIO A1 ****************************************/

/**
  * @brief  PA1 ��ʼ�������� LED2
  */
void GPIO_A1_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE(); //���� PA ʱ��

    GPIO_Initure.Pin = GPIO_PIN_1;                  //PA1
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;        //�������
    GPIO_Initure.Pull = GPIO_PULLUP;                //����
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; //����
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);            //��ʼ�� PA1
}

/****************************************************************************************/

/************************************** GPIO E3 C5 **************************************/

/**
  * @brief  PE3 PC5 ��ʼ�������� KEY1 KEY2
  */
void GPIO_E3_C5_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE(); //���� PC ʱ��
    __HAL_RCC_GPIOE_CLK_ENABLE(); //���� PE ʱ��

    GPIO_Initure.Pin = GPIO_PIN_3;                  //PE3����Ӧ K1
    GPIO_Initure.Mode = GPIO_MODE_INPUT;            //����
    GPIO_Initure.Pull = GPIO_PULLUP;                //����
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; //����
    HAL_GPIO_Init(GPIOE, &GPIO_Initure);            //��ʼ�� PE3

    GPIO_Initure.Pin = GPIO_PIN_5;                  //PC5����Ӧ K2
    GPIO_Initure.Mode = GPIO_MODE_INPUT;            //����
    GPIO_Initure.Pull = GPIO_PULLUP;                //����
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; //����
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);            //��ʼ�� PC5
}

/****************************************************************************************/

/************************************ GPIO B0 B1 B12 ************************************/

/**
  * @brief  PB0 PB1 PB12 ��ʼ�������� LCD �� BLK RS CS
  */
void GPIO_B0_B1_B12_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE(); //���� PB ʱ��

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_12; //PB0 PB1 PB12
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;                  //�������
    GPIO_Initure.Pull = GPIO_PULLUP;                          //����
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;           //����
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);                      //��ʼ�� PB0 PB1 PB12
}

/****************************************************************************************/
