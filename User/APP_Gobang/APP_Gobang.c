//五子棋应用

#include "stdlib.h"

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

#define CONNETING_STATUS 0
#define BATTLING_STATUS 1

#define NONE_MSG 0
#define REQUEST_MSG 'R'
#define AGREE_MSG 'A'
#define REFUSE_MSG 'N'

#define START_BYTE '#'
#define END_BYTE '&'

#define Random() rand()

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

//我方执子类型
int my_turn = BLACK_TURN;

//当前所处状态
int status = CONNETING_STATUS;

//联机消息
uint8_t connetion_msg = NONE_MSG;

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
void APP_Gobang_SendPosition(void);
void APP_Gobang_RevPosition(void);
uint8_t APP_Gobang_Connect(void);
uint8_t APP_Gobang_InitiateConnet(void);
uint8_t APP_Gobang_ReplyConnect(void);
void APP_Gobang_DecideMyTurn(void);
void APP_Gobang_RevMyTurn(void);
void APP_Gobang_ConnectRevHandler(uint8_t byte);

/*****************************************************************************************/

/**
  * @brief  启动五子棋
  */
void APP_Gobang_Launcher(void)
{
    srand(SysTick->VAL);
    APP_Gobang_Init();
    APP_Gobang_DispGobang();
    if (APP_Gobang_Connect() == 0)
        return;
    status = BATTLING_STATUS;
    HC12_BindReceiveHandle(NULL);
    HC12_ClearReceive;
    KEY_ClearKey();

    while (1)
    {
        APP_Gobang_DispGobang();

        if (turn == my_turn)
            APP_Gobang_MoveChess();
        else
            APP_Gobang_RevPosition();

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
    status = CONNETING_STATUS;
    connetion_msg = NONE_MSG;

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
    APP_Gobang_DispMap();
    APP_Gobang_DispChess();
    APP_Gobang_DispCursor();
    if (status == BATTLING_STATUS)
        APP_Gobang_DispText();
    GE_Draw_Disp();
}

/**
  * @brief  绘制文字
  * @param  head: 标题
  * @param  content: 内容
  */
void APP_Gobang_DispText(void)
{
    if (turn == my_turn)
        GE_Font_Print_WithSet(LCD_WIDTH - FONT_16 * 2, interval / 2, BORDER_MAX, BORDER_MAX, "我方");
    else
        GE_Font_Print_WithSet(LCD_WIDTH - FONT_16 * 2, interval / 2, BORDER_MAX, BORDER_MAX, "对方");
    GE_Font_Print_WithSet(LCD_WIDTH - FONT_16 * 2, interval / 2 + FONT_16, BORDER_MAX, BORDER_MAX, "回合");

    GE_Font_Print_WithSet(LCD_WIDTH - FONT_16 * 2, LCD_HEIGHT - interval / 2 - 2 * FONT_16, BORDER_MAX, BORDER_MAX, "我方");
    if (my_turn == BLACK_TURN)
        GE_Font_Print_WithSet(LCD_WIDTH - FONT_16 * 2, LCD_HEIGHT - interval / 2 - FONT_16, BORDER_MAX, BORDER_MAX, "执黑");
    else
        GE_Font_Print_WithSet(LCD_WIDTH - FONT_16 * 2, LCD_HEIGHT - interval / 2 - FONT_16, BORDER_MAX, BORDER_MAX, "执白");
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
            APP_Gobang_SendPosition();
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

void APP_Gobang_SendPosition(void)
{
    uint8_t position[6];
    position[0] = START_BYTE;
    position[1] = '0' + cursor_x / 10;
    position[2] = '0' + cursor_x % 10;
    position[3] = '0' + cursor_y / 10;
    position[4] = '0' + cursor_y % 10;
    position[5] = END_BYTE;

    HC12_SendBuff(position, 6);
}

void APP_Gobang_RevPosition(void)
{
    uint8_t byte;
    uint8_t position[5];
    while (1)
    {
        if (HC12_Receive(&byte) == 1)
        {
            if (byte == START_BYTE)
            {
                HC12_ReceiveBuffUntil(position, END_BYTE, 500);
                cursor_x = (position[0] - '0') * 10 + position[1] - '0';
                cursor_y = (position[2] - '0') * 10 + position[3] - '0';

                if (map[cursor_x][cursor_y] == NO_CHESS)
                {
                    chess_kind = map[cursor_x][cursor_y] = turn == BLACK_TURN ? BLACK_CHESS : WHITE_CHESS;
                    APP_Gobang_DispGobang();
                    break;
                }
            }
        }
    }
}

uint8_t APP_Gobang_Connect(void)
{
    HC12_BindReceiveHandle(APP_Gobang_ConnectRevHandler);

    GE_Draw_Fill(60, 75, 200, 90, WHITE);
    GE_GUI_MsgBox(60, 75, 200, 90, "当前未联机", "按“OK”以发起联机", NULL);

    while (1)
    {
        switch (KEY_GetKey())
        {
        case JOY_OK_DOWN:
        {
            if (APP_Gobang_InitiateConnet())
            {
                uint8_t *head = (my_turn == BLACK_TURN ? "本局我方执黑" : "本局我方执白");
                GE_Draw_Fill(60, 75, 200, 90, WHITE);
                GE_GUI_MsgBox(60, 75, 200, 90, head, "按“OK”开始游戏", NULL);
                Delay_ms(500);
                KEY_WaitKey(JOY_OK);
                return 1;
            }
            else
            {
                GE_Draw_Fill(60, 75, 200, 90, WHITE);
                GE_GUI_MsgBox(60, 75, 200, 90, "当前未联机", "按“OK”以发起联机", NULL);
            }
        }
        break;

        case JOY_L_DOWN:
        {
            return 0;
        }
        break;
        }

        if (connetion_msg == REQUEST_MSG)
        {
            if (APP_Gobang_ReplyConnect())
            {
                uint8_t *head = (my_turn == BLACK_TURN ? "本局我方执黑" : "本局我方执白");
                GE_Draw_Fill(60, 75, 200, 90, WHITE);
                GE_GUI_MsgBox(60, 75, 200, 90, head, "按“OK”开始游戏", NULL);
                Delay_ms(200);
                KEY_WaitKey(JOY_OK);
                return 1;
            }
            else
            {
                GE_Draw_Fill(60, 75, 200, 90, WHITE);
                GE_GUI_MsgBox(60, 75, 200, 90, "当前未联机", "按“OK”以发起联机", NULL);
            }
        }
    }
}

uint8_t APP_Gobang_InitiateConnet(void)
{
    int32_t start_time = SysTick_GetRunTime();

    uint8_t msg[] = {START_BYTE, REQUEST_MSG};
    HC12_SendBuff(msg, 2);
    HC12_ClearReceive;

    GE_Draw_Fill(60, 75, 200, 90, WHITE);
    GE_GUI_MsgBox(60, 75, 200, 90, "当前未联机", "请求已发送，等待对方响应...", NULL);

    while (1)
    {
        if (SysTick_CheckRunTime(start_time) > 10000)
        {
            GE_Draw_Fill(60, 75, 200, 90, WHITE);
            GE_GUI_MsgBox(60, 75, 200, 90, "联机失败", "对方无响应,按“OK”退出", NULL);
            KEY_WaitKey(JOY_OK);
            connetion_msg = NONE_MSG;
            return 0;
        }

        if (connetion_msg == AGREE_MSG)
        {
            //APP_Gobang_RevMyTurn();
            GE_Draw_Fill(60, 75, 200, 90, WHITE);
            GE_GUI_MsgBox(60, 75, 200, 90, "联机成功", "按“OK”进入游戏", NULL);
            KEY_WaitKey(JOY_OK);
            connetion_msg = NONE_MSG;
            return 1;
        }

        else if (connetion_msg == REFUSE_MSG)
        {
            GE_Draw_Fill(60, 75, 200, 90, WHITE);
            GE_GUI_MsgBox(60, 75, 200, 90, "联机失败", "对方拒绝联机,按“OK”退出", NULL);
            KEY_WaitKey(JOY_OK);
            connetion_msg = NONE_MSG;
            return 0;
        }
    }
}

uint8_t APP_Gobang_ReplyConnect(void)
{
    uint8_t content[2][GE_GUI_MENUBOX_CONTENT_LEN] = {"是", "否"};
    GE_Draw_Fill(60, 75, 200, 90, WHITE);

    switch (GE_GUI_MenuBox(60, 75, 200, 90, "对方请求联机，是否接受？", 2, content, NULL))
    {
    case 0:
        return 0;
        break;

    case 1:
    {
        uint8_t msg[] = {START_BYTE, AGREE_MSG};
        HC12_SendBuff(msg, 2);
        APP_Gobang_DecideMyTurn();

        GE_Draw_Fill(60, 75, 200, 90, WHITE);
        GE_GUI_MsgBox(60, 75, 200, 90, "联机成功", "按“OK”进入游戏", NULL);
        KEY_WaitKey(JOY_OK);
        return 1;
    }
    break;

    case 2:
    {
        uint8_t msg[] = {START_BYTE, REFUSE_MSG};
        HC12_SendBuff(msg, 2);
        GE_Draw_Fill(60, 75, 200, 90, WHITE);
        GE_GUI_MsgBox(60, 75, 200, 90, "联机失败", "已拒绝联机,按“OK”退出", NULL);
        KEY_WaitKey(JOY_OK);
        return 0;
    }
    break;
    }
}

void APP_Gobang_DecideMyTurn(void)
{
    my_turn = Random() % 2;

    uint8_t msg[] = {START_BYTE, !my_turn};
    HC12_SendBuff(msg, 2);
}

void APP_Gobang_ConnectRevHandler(uint8_t byte)
{
    static uint8_t is_receiving = FALSE;

    if (is_receiving == FALSE && byte == START_BYTE)
    {
        is_receiving = TRUE;
        return;
    }

    if (is_receiving == TRUE)
    {
        if (byte == REQUEST_MSG || byte == AGREE_MSG || byte == REFUSE_MSG)
        {
            connetion_msg = byte;
            is_receiving = FALSE;
        }
        else if (byte == BLACK_TURN || byte == WHITE_TURN)
        {
            my_turn = byte;
            is_receiving = FALSE;
        }
    }
}