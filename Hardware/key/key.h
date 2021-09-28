//按键 FIFO 库

#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

//按键事件别名

#define KEY1_DOWN KEY_1_DOWN
#define KEY1_UP KEY_1_UP
#define KEY1_LONG KEY_1_LONG

#define KEY2_DOWN KEY_2_DOWN
#define KEY2_UP KEY_2_UP
#define KEY2_LONG KEY_2_LONG

#define JOY_OK_DOWN KEY_3_DOWN //OK
#define JOY_OK_UP KEY_3_UP
#define JOY_OK_LONG KEY_3_LONG

#define JOY_U_DOWN KEY_4_DOWN //上
#define JOY_U_UP KEY_4_UP
#define JOY_U_LONG KEY_4_LONG

#define JOY_D_DOWN KEY_5_DOWN //下
#define JOY_D_UP KEY_5_UP
#define JOY_D_LONG KEY_5_LONG

#define JOY_L_DOWN KEY_6_DOWN //左
#define JOY_L_UP KEY_6_UP
#define JOY_L_LONG KEY_6_LONG

#define JOY_R_DOWN KEY_7_DOWN //右
#define JOY_R_UP KEY_7_UP
#define JOY_R_LONG KEY_7_LONG

//按键 ID
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

#define KEY_FILTER_TIME 5 //按键滤波时间 50ms，单位 10ms
#define KEY_LONG_TIME 100 //持续 1 秒，长按事件

typedef struct
{
    uint8_t (*IsKeyDownFunc)(void); //按键按下的判断函数，1 表示按下

    uint8_t Count;       //滤波器计数器
    uint16_t LongCount;  //长按计数器
    uint16_t LongTime;   //按键按下持续时间，0 表示不检测长按
    uint8_t State;       //按键当前状态（按下还是弹起）
    uint8_t RepeatSpeed; //连续按键周期
    uint8_t RepeatCount; //连续按键计数器
} KEY_T;

typedef enum
{
    KEY_NONE = 0,

    KEY_1_DOWN, //1 键按下
    KEY_1_UP,   //1 键弹起
    KEY_1_LONG, //1 键长按

    KEY_2_DOWN, //2 键按下
    KEY_2_UP,   //2 键弹起
    KEY_2_LONG, //2 键长按

    KEY_3_DOWN, //3 键按下
    KEY_3_UP,   //3 键弹起
    KEY_3_LONG, //3 键长按

    KEY_4_DOWN, //4 键按下
    KEY_4_UP,   //4 键弹起
    KEY_4_LONG, //4 键长按

    KEY_5_DOWN, //5 键按下
    KEY_5_UP,   //5 键弹起
    KEY_5_LONG, //5 键长按

    KEY_6_DOWN, //6 键按下
    KEY_6_UP,   //6 键弹起
    KEY_6_LONG, //6 键长按

    KEY_7_DOWN, //7 键按下
    KEY_7_UP,   //7 键弹起
    KEY_7_LONG, //7 键长按
} KEY_ENUM;

//按键 FIFO 变量
#define KEY_FIFO_SIZE 10
typedef struct
{
    uint8_t Buf[KEY_FIFO_SIZE]; //键值缓冲区
    uint8_t Read;               //缓冲区读指针
    uint8_t Write;              //缓冲区写指针
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
