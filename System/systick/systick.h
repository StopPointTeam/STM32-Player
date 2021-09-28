//SysTick �������ʱ����

#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "sys.h"

#define TMR_COUNT 4 //�����ʱ���ĸ���

//��ʱ���ṹ�幤��ģʽ
typedef enum
{
    TMR_ONCE_MODE = 0, //һ�ι���ģʽ
    TMR_AUTO_MODE = 1  //�Զ���ʱ����ģʽ
} TMR_MODE_E;

//��ʱ���ṹ��
typedef struct
{
    volatile uint8_t Mode;     //������ģʽ
    volatile uint8_t Flag;     //��ʱ�����־
    volatile uint32_t Count;   //������
    volatile uint32_t PreLoad; //������Ԥװֵ
} SOFT_TMR;

void SysTick_Init(void);
void Delay_ms(uint32_t n);
void Delay_us(uint32_t n);
void SysTick_StartTimer(uint8_t _id, uint32_t _period);
void SysTick_StartAutoTimer(uint8_t _id, uint32_t _period);
void SysTick_StopTimer(uint8_t _id);
uint8_t SysTick_CheckTimer(uint8_t _id);
int32_t SysTick_GetRunTime(void);
int32_t SysTick_CheckRunTime(int32_t _LastTime);

#endif
