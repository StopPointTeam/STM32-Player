//游戏引擎图像绘制库

#ifndef __GE_DRAW_H
#define __GE_DRAW_H

#include "sys.h"

//颜色宏定义
#define BLACK 0x0000
#define NAVY 0x000f
#define DARK_GREEN 0x03e0
#define DARK_CYAN 0x03ef
#define MAROON 0x7800
#define PURPLE 0x780f
#define OLIVE 0x7be0
#define LIGHT_GREY 0xc618
#define DARK_GREY 0x7bef
#define BLUE 0x001f
#define GREEN 0x07e0
#define CYAN 0x07ff
#define RED 0xf800
#define MAGENTA 0xf81f
#define YELLOW 0xffe0
#define WHITE 0xffff

/****************************************   显存   ****************************************/

extern uint8_t GE_Draw_VRam[153600];

/*****************************************************************************************/

/***************************************** 初始化 *****************************************/

void GE_Draw_Init(void);

/*****************************************************************************************/

/**************************************** 刷新屏幕 ****************************************/

void GE_Draw_Disp(void);

/*****************************************************************************************/

/**************************************** 清空屏幕 ****************************************/

void GE_Draw_ClrAll(uint16_t color);

/*****************************************************************************************/

/**************************************** 绘图函数 ****************************************/

void GE_Draw_Fill(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color);
void GE_Draw_Point(int16_t x, int16_t y, uint16_t color);
uint16_t GE_Draw_GetPoint(int16_t x, int16_t y);
void GE_Draw_Line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void GE_Draw_Rectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color);
void GE_Draw_Circle(int16_t xm, int16_t ym, uint16_t r, uint16_t color);
void GE_Draw_FillCircle(int16_t xm, int16_t ym, uint16_t r, uint16_t color);

/*****************************************************************************************/

/**************************************** 绘制图片 ****************************************/
//显示位置
#define UP_LEFT 0 //左上角
#define MID 1     //中心

typedef struct
{
    uint8_t is_reverse; //是否反色
    uint8_t pos_mode;   //显示位置
} _ge_draw_pic_set;

extern _ge_draw_pic_set ge_draw_pic_set;

void GE_Draw_Pic(
    int16_t x,
    int16_t y,
    uint8_t is_reverse,
    uint8_t pos_mode,
    const unsigned char *pic,
    uint16_t width,
    uint16_t height);
void GE_Draw_Pic_WithSet(int16_t x, int16_t y, const unsigned char *pic, uint16_t width, uint16_t height);

/*****************************************************************************************/

/************************************** 绘制单色位图 **************************************/
//是否绘制背景色
#define MONO_OR 0
#define MONO_AND 1
#define MONO_XOR 2
#define MONO_COVER 3

void GE_Draw_Mono(
    int16_t x,
    int16_t y,
    uint8_t draw_mode,
    uint8_t pos_mode,
    uint16_t mono_color,
    uint16_t back_color,
    const unsigned char *pic,
    uint16_t width,
    uint16_t height);

typedef struct
{
    uint8_t draw_mode;   //绘制方式
    uint8_t pos_mode;    //显示位置
    uint16_t mono_color; //颜色
    uint16_t back_color; //背景颜色
} _ge_draw_mono_set;

extern _ge_draw_mono_set ge_draw_mono_set;

void GE_Draw_Mono_WithSet(
    int16_t x,
    int16_t y,
    const unsigned char *pic,
    uint16_t width,
    uint16_t height);

/*****************************************************************************************/

#endif
