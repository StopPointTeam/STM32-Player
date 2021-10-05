//移动球应用

#include "stdio.h"

#include "sys.h"
#include "systick.h"

#include "key.h"
#include "lcd.h"
#include "hc12.h"

#include "GameEngine.h"

#include "APP_Ball.h"

/**************************************** 私有定义 ****************************************/

#define START_BYTE '&'

typedef struct
{
    uint16_t x;
    uint16_t y;
} Ball;

/*****************************************************************************************/

/**************************************** 全局变量 ****************************************/

Ball ball;

/*****************************************************************************************/

/**************************************** 私有函数 ****************************************/

void APP_Ball_DispBall(void);
void APP_Ball_Send(void);
void APP_Ball_Receive(uint8_t byte);
void WriteMem(void *pdata, uint32_t len, uint8_t *list);

/*****************************************************************************************/

/**
  * @brief  启动移动球
  */
void APP_Ball_Launcher(void)
{
    HC12_BindReceiveHandle(APP_Ball_Receive);

    ball.x = 0;
    ball.y = 0;

    APP_Ball_DispBall();

    while (1)
    {
        switch (KEY_GetKeyWait())
        {
        case JOY_U_DOWN:
        {
            if (ball.y - 2 >= 0)
            {
                ball.y -= 2;
                APP_Ball_DispBall();

                APP_Ball_Send();
            }
        }
        break;

        case JOY_D_DOWN:
        {
            if (ball.y + 2 <= LCD_HEIGHT)
            {
                ball.y += 2;
                APP_Ball_DispBall();

                APP_Ball_Send();
            }
        }
        break;

        case JOY_L_DOWN:
        {
            if (ball.x - 2 >= 0)
            {
                ball.x -= 2;
                APP_Ball_DispBall();

                APP_Ball_Send();
            }
        }
        break;

        case JOY_R_DOWN:
        {
            if (ball.x + 2 <= LCD_WIDTH)
            {
                ball.x += 2;
                APP_Ball_DispBall();

                APP_Ball_Send();
            }
        }
        }
    }
}

void APP_Ball_DispBall(void)
{
    GE_Draw_ClrAll(WHITE);
    GE_Draw_FillCircle(ball.x, ball.y, 10, BLUE);
    GE_Draw_Disp();
}

void APP_Ball_Send(void)
{
    Ball pack;
    pack.x = ball.x;
    pack.y = ball.y;

    char start_byte = START_BYTE;
    HC12_SendBuff(&start_byte, sizeof(start_byte));
    HC12_SendBuff(&pack, sizeof(Ball));
}

void APP_Ball_Receive(uint8_t byte)
{
    static uint8_t is_receiving = FALSE;
    static uint8_t list[4];
    static uint8_t list_i = 0;

    if (is_receiving == FALSE && byte == START_BYTE)
    {
        is_receiving = TRUE;
        return;
    }

    if (is_receiving == TRUE)
    {
        list[list_i] = byte;
        list_i++;

        if (list_i == 4)
        {
            WriteMem(&ball, sizeof(Ball), list);
            APP_Ball_DispBall();
            list_i = 0;
            is_receiving = FALSE;

            printf("收到小球位置：x=%d y=%d\n", ball.x, ball.y);
        }
    }
}

void WriteMem(void *pdata, uint32_t len, uint8_t *list)
{
    uint8_t *p = (uint8_t *)pdata;

    for (uint32_t i = 0; i < len; i++)
        p[i] = list[i];
}
