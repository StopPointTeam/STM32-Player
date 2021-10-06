//五子棋应用

#include "sys.h"
#include "systick.h"

#include "key.h"
#include "lcd.h"
#include "hc12.h"

#include "GameEngine.h"

#include "APP_Gobang.h"

/**************************************** 私有定义 ****************************************/

#define MAP_COLOR 0xdc84

#define BROADWIDTH 15
#define WIDTH (BROADWIDTH + 1)

#define BLACK_TURN 0
#define WHITE_TURN 1

#define NO_CHESS 0
#define BLACK_CHESS 1
#define WHITE_CHESS 2

#define CHECK_X 0
#define CHECK_Y 1
#define CHECK_DIAG_LEFT 2
#define CHECK_DIAG_RIGHT 3

#define CHESS_RADIUS 5

/*****************************************************************************************/

/**************************************** 全局变量 ****************************************/
//棋盘交点阵
int map[WIDTH][WIDTH] = {{0, 0}};

//光标位置
int cursor_x = 7;
int cursor_y = 7;

//记录执子方
int turn = BLACK_TURN;

//棋子类型
int chess_kind = BLACK_CHESS;

uint8_t interval = 240 / WIDTH;
uint16_t x_start, x_end, y_start, y_end;

/*****************************************************************************************/

/**************************************** 私有函数 ****************************************/

void APP_Gobang_Init(void);
void APP_Gobang_DispGobang(void);
void APP_Gobang_DispText(void);
void APP_Gobang_DispMap(void);
void APP_Gobang_DispChess(void);
void APP_Gobang_MoveChess(void);
void APP_Gobang_DispCursor(void);
int APP_Gobang_CheckNum(int check_type);
void APP_Gobang_Msg(uint8_t *head, uint8_t *content);

/*****************************************************************************************/

/**
  * @brief  启动五子棋
  */
void APP_Gobang_Launcher(void)
{
    APP_Gobang_Init();

    while (1)
    {
        APP_Gobang_DispGobang();
        APP_Gobang_MoveChess();
        turn = !turn; //更换执子方

        //判断胜负
        if (
            APP_Gobang_CheckNum(CHECK_X) >= 5 ||
            APP_Gobang_CheckNum(CHECK_Y) >= 5 ||
            APP_Gobang_CheckNum(CHECK_DIAG_LEFT) >= 5 ||
            APP_Gobang_CheckNum(CHECK_DIAG_RIGHT) >= 5)
        {
            if (chess_kind == BLACK_CHESS ? 1 : 0)
                APP_Gobang_Msg("游戏结束", "黑棋获得胜利！");
            else
                APP_Gobang_Msg("游戏结束", "白棋获得胜利！");

            return;
        }
    }
}

/**
  * @brief  初始化变量
  */
void APP_Gobang_Init(void)
{
    ge_font_print_set.font_size = FONT_16;

    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < WIDTH; j++)
            map[i][j] = NO_CHESS;

    cursor_x = cursor_y = 7;
    turn = BLACK_TURN;
    chess_kind = BLACK_CHESS;

    x_start = (LCD_WIDTH - LCD_HEIGHT) / 2 + interval / 2;
    x_end = x_start + (WIDTH - 1) * interval;
    y_start = interval / 2;
    y_end = y_start + (WIDTH - 1) * interval;
}

/**
  * @brief  绘制游戏
  */
void APP_Gobang_DispGobang(void)
{
    GE_Draw_ClrAll(MAP_COLOR);
    APP_Gobang_DispText();
    APP_Gobang_DispMap();
    APP_Gobang_DispChess();
    APP_Gobang_DispCursor();
    GE_Draw_Disp();
}

/**
  * @brief  绘制文字
  * @param  head: 标题
  * @param  content: 内容
  */
void APP_Gobang_DispText(void)
{
    if (turn == BLACK_TURN)
        GE_Font_Print_WithSet(LCD_WIDTH - FONT_16 * 2, interval / 2, BORDER_MAX, BORDER_MAX, "黑方");
    else
        GE_Font_Print_WithSet(LCD_WIDTH - FONT_16 * 2, interval / 2, BORDER_MAX, BORDER_MAX, "白方");

    GE_Font_Print_WithSet(LCD_WIDTH - FONT_16 * 2, interval / 2 + FONT_16, BORDER_MAX, BORDER_MAX, "执子");
}

/**
  * @brief  绘制棋盘
  */
void APP_Gobang_DispMap(void)
{
    uint8_t i = 0;
    for (i = 0; i < WIDTH; i++)
    {
        GE_Draw_Line(x_start, y_start + i * interval, x_end, y_start + i * interval, BLACK);
        GE_Draw_Line(x_start + i * interval, y_start, x_start + i * interval, y_end, BLACK);
    }
}

/**
  * @brief  绘制棋子
  */
void APP_Gobang_DispChess(void)
{
    uint8_t i, j;
    for (i = 0; i < WIDTH; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if (map[i][j] == BLACK_CHESS)
            {
                GE_Draw_FillCircle(x_start + j * interval, y_start + i * interval, CHESS_RADIUS, BLACK);
            }
            else if (map[i][j] == WHITE_CHESS)
            {
                GE_Draw_FillCircle(x_start + j * interval, y_start + i * interval, CHESS_RADIUS, WHITE);
            }
        }
    }
}

/**
  * @brief  移动棋子
  */
void APP_Gobang_MoveChess(void)
{
    while (1)
    {
        switch (KEY_GetKeyWait())
        {
        case JOY_U_DOWN:
        {
            cursor_x--;

            if (cursor_x < 0)
                cursor_x = WIDTH - 1;

            APP_Gobang_DispGobang();
        }
        break;

        case JOY_D_DOWN:
        {
            cursor_x++;

            if (cursor_x > WIDTH - 1)
                cursor_x = 0;

            APP_Gobang_DispGobang();
        }
        break;

        case JOY_L_DOWN:
        {
            cursor_y--;

            if (cursor_y < 0)
                cursor_y = WIDTH - 1;

            APP_Gobang_DispGobang();
        }
        break;

        case JOY_R_DOWN:
        {
            cursor_y++;

            if (cursor_y > WIDTH - 1)
                cursor_y = 0;

            APP_Gobang_DispGobang();
        }
        break;

        case JOY_OK_DOWN:
        {
            if (map[cursor_x][cursor_y] != NO_CHESS)
                break;

            chess_kind = map[cursor_x][cursor_y] = turn == BLACK_TURN ? BLACK_CHESS : WHITE_CHESS;
            APP_Gobang_DispGobang();
            return;
        }
        }
    }
}

/**
  * @brief  显示光标
  */
void APP_Gobang_DispCursor(void)
{
    GE_Draw_Circle(x_start + cursor_y * interval, y_start + cursor_x * interval, CHESS_RADIUS, RED);
}

/**
  * @brief  检查某个方向上的连子数
  * @param  check_type: 检查方向
  * @retval 连子数
  */
int APP_Gobang_CheckNum(int check_type)
{
    int temp_x = cursor_x, temp_y = cursor_y;
    int num = 0;

    while (temp_x >= 0 && temp_x < WIDTH && temp_y >= 0 && temp_y < WIDTH && map[temp_x][temp_y] == chess_kind)
    {
        num++;

        switch (check_type)
        {
        case CHECK_X:
            temp_x--;
            break;
        case CHECK_Y:
            temp_y--;
            break;
        case CHECK_DIAG_LEFT:
            temp_x--;
            temp_y--;
            break;
        case CHECK_DIAG_RIGHT:
            temp_x--;
            temp_y++;
        }
    }

    switch (check_type)
    {
    case CHECK_X:
        temp_x = cursor_x + 1;
        break;
    case CHECK_Y:
        temp_y = cursor_y + 1;
        ;
        break;
    case CHECK_DIAG_LEFT:
        temp_x = cursor_x + 1;
        temp_y = cursor_y + 1;
        break;
    case CHECK_DIAG_RIGHT:
        temp_x = cursor_x + 1;
        temp_y = cursor_y - 1;
    }

    while (temp_x >= 0 && temp_x < WIDTH && temp_y >= 0 && temp_y < WIDTH && map[temp_x][temp_y] == chess_kind)
    {
        num++;

        switch (check_type)
        {
        case CHECK_X:
            temp_x++;
            break;
        case CHECK_Y:
            temp_y++;
            break;
        case CHECK_DIAG_LEFT:
            temp_x++;
            temp_y++;
            break;
        case CHECK_DIAG_RIGHT:
            temp_x++;
            temp_y--;
        }
    }

    return num;
}

/**
  * @brief  消息框，任意键按下后退出
  * @param  head: 标题
  * @param  content: 内容
  */
void APP_Gobang_Msg(uint8_t *head, uint8_t *content)
{
    GE_Draw_Fill(60, 75, 200, 90, WHITE);
    GE_GUI_MsgBox(60, 75, 200, 90, head, content, NULL);
    KEY_WaitKey(JOY_L);
}
