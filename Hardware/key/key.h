//���� FIFO ��

#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

//�����¼�����

#define KEY1_DOWN KEY_1_DOWN
#define KEY1_UP KEY_1_UP
#define KEY1_LONG KEY_1_LONG

#define KEY2_DOWN KEY_2_DOWN
#define KEY2_UP KEY_2_UP
#define KEY2_LONG KEY_2_LONG

#define JOY_OK_DOWN KEY_3_DOWN //OK
#define JOY_OK_UP KEY_3_UP
#define JOY_OK_LONG KEY_3_LONG

#define JOY_U_DOWN KEY_4_DOWN //��
#define JOY_U_UP KEY_4_UP
#define JOY_U_LONG KEY_4_LONG

#define JOY_D_DOWN KEY_5_DOWN //��
#define JOY_D_UP KEY_5_UP
#define JOY_D_LONG KEY_5_LONG

#define JOY_L_DOWN KEY_6_DOWN //��
#define JOY_L_UP KEY_6_UP
#define JOY_L_LONG KEY_6_LONG

#define JOY_R_DOWN KEY_7_DOWN //��
#define JOY_R_UP KEY_7_UP
#define JOY_R_LONG KEY_7_LONG

//���� ID
typedef enum
{
    KEY1 = 0,
    KEY2,
    JOY_OK,
    JOY_U,
    JOY_D,
    JOY_L,
    JOY_R
} KEY_ID_E;

#define KEY_FILTER_TIME 5 //�����˲�ʱ�� 50ms����λ 10ms
#define KEY_LONG_TIME 100 //���� 1 �룬�����¼�

typedef struct
{
    uint8_t (*IsKeyDownFunc)(void); //�������µ��жϺ�����1 ��ʾ����

    uint8_t Count;       //�˲���������
    uint16_t LongCount;  //����������
    uint16_t LongTime;   //�������³���ʱ�䣬0 ��ʾ����ⳤ��
    uint8_t State;       //������ǰ״̬�����»��ǵ���
    uint8_t RepeatSpeed; //������������
    uint8_t RepeatCount; //��������������
} KEY_T;

typedef enum
{
    KEY_NONE = 0,

    KEY_1_DOWN, //1 ������
    KEY_1_UP,   //1 ������
    KEY_1_LONG, //1 ������

    KEY_2_DOWN, //2 ������
    KEY_2_UP,   //2 ������
    KEY_2_LONG, //2 ������

    KEY_3_DOWN, //3 ������
    KEY_3_UP,   //3 ������
    KEY_3_LONG, //3 ������

    KEY_4_DOWN, //4 ������
    KEY_4_UP,   //4 ������
    KEY_4_LONG, //4 ������

    KEY_5_DOWN, //5 ������
    KEY_5_UP,   //5 ������
    KEY_5_LONG, //5 ������

    KEY_6_DOWN, //6 ������
    KEY_6_UP,   //6 ������
    KEY_6_LONG, //6 ������

    KEY_7_DOWN, //7 ������
    KEY_7_UP,   //7 ������
    KEY_7_LONG, //7 ������
} KEY_ENUM;

//���� FIFO ����
#define KEY_FIFO_SIZE 10
typedef struct
{
    uint8_t Buf[KEY_FIFO_SIZE]; //��ֵ������
    uint8_t Read;               //��������ָ��
    uint8_t Write;              //������дָ��
} KEY_FIFO_T;

void KEY_Init(void);
void KEY_Scan10ms(void);
void KEY_PutKey(uint8_t _KeyCode);
uint8_t KEY_GetKey(void);
uint8_t KEY_GetKeyWait(void);
uint8_t KEY_GetKeyState(KEY_ID_E _ucKeyID);
void KEY_WaitKey(KEY_ID_E _ucKeyID);
void KEY_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t _RepeatSpeed);
void KEY_ClearKey(void);

#endif
