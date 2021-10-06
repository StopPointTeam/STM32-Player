//五子棋应用

#include "stdio.h"

#include "sys.h"
#include "systick.h"

#include "key.h"
#include "lcd.h"
#include "hc12.h"

#include "GameEngine.h"

#include "APP_Gobang.h"

/**************************************** 私有定义 ****************************************/

#define BROADWIDTH 15
#define WIDTH (BROADWIDTH + 1)

#define BLACK_TURN 0
#define WHITE_TURN 1

#define NO_CHESS 0
#define BLACK_CHESS 1
#define WHITE_CHESS 2

#define NONE_WIN 0
#define BLACK_WIN 1
#define WHITE_WIN 2

#define CHECK_X 0
#define CHECK_Y 1
#define CHECK_DIAG_LEFT 2
#define CHECK_DIAG_RIGHT 3

#define CHESS_RADIUS 5

typedef struct
{
    uint8_t x;
    uint8_t y;
} Chess;

/*****************************************************************************************/

/**************************************** 全局变量 ****************************************/

Chess put_chess = {0, 0};
Chess move_chess = {0, 0};

//棋盘交点阵
int map[WIDTH][WIDTH] = {{0, 0}};

//记录执子方
int turn = BLACK_TURN;

//棋子类型
int chess_kind = BLACK_CHESS;

uint8_t interval = 240 / WIDTH;
uint16_t x_start, x_end, y_start, y_end;

/*****************************************************************************************/

/**************************************** 私有函数 ****************************************/

void APP_Gobang_DispGobang(void);

void APP_Gobang_DispMap(void);

void APP_Gobang_DispChess(void);

/*****************************************************************************************/

/**
  * @brief  启动五子棋
  */
void APP_Gobang_Launcher(void)
{
    x_start = (320 - 240) / 2 + interval / 2;
    x_end = x_start + (WIDTH - 1) * interval;
    y_start = interval / 2;
    y_end = y_start + (WIDTH - 1) * interval;

    uint8_t byte;
    uint8_t list[4];

    APP_Gobang_DispGobang();

    while (1)
    {
    }
}

void APP_Gobang_DispGobang(void)
{
    GE_Draw_ClrAll(WHITE);
    APP_Gobang_DispMap();
    APP_Gobang_DispChess();
    GE_Draw_Disp();
}

void APP_Gobang_DispMap(void)
{
    uint8_t i = 0;
    for (i = 0; i < WIDTH; i++)
    {
        GE_Draw_Line(x_start, y_start + i * interval, x_end, y_start + i * interval, BLACK);
        GE_Draw_Line(x_start + i * interval, y_start, x_start + i * interval, y_end, BLACK);
    }
}

void APP_Gobang_DispChess(void)
{
    uint8_t i, j;
    for (i = 0; i < WIDTH; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if (map[i][j] == BLACK_CHESS)
            {
                GE_Draw_FillCircle(x_start + j * interval, y_start + i * interval, CHESS_RADIUS, RED);
            }
            else if (map[i][j] == WHITE_CHESS)
            {
                GE_Draw_FillCircle(x_start + j * interval, y_start + i * interval, CHESS_RADIUS, BLUE);
            }
        }
    }
}
