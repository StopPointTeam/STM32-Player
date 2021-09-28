//fxlib 兼容层，用于从 fx-9860 平台向 STM32-Player 移植程序

#include "sys.h"
#include "systick.h"

#include "led.h"
#include "key.h"
#include "lcd.h"

#include "GameEngine.h"

#include "fxlib.h"

/**************************************** 私有函数 ****************************************/

void FX_Draw_Point(int16_t x, int16_t y, uint16_t color);

uint16_t FX_Draw_GetPoint(int16_t x, int16_t y);

void FX_Draw_Mono(
    int16_t x,
    int16_t y,
    uint8_t draw_mode,
    uint8_t pos_mode,
    uint16_t mono_color,
    uint16_t back_color,
    const unsigned char *pic,
    uint16_t width,
    uint16_t height);

/*****************************************************************************************/

/***************************************** FxLib *****************************************/

void Sleep(int millisecond)
{
    Delay_ms(millisecond);
}

void ML_clear_vram()
{
    GE_Draw_ClrAll(WHITE);
}

void ML_clear_screen()
{
    GE_Draw_ClrAll(WHITE);
    GE_Draw_Disp();
}

void ML_display_vram()
{
    GE_Draw_Disp();
}

void ML_bmp_or(const unsigned char *bmp, int x, int y, int width, int height)
{
    FX_Draw_Mono(x, y, MONO_OR, UP_LEFT, BLACK, WHITE, bmp, width, height);
}

void ML_bmp_and(const unsigned char *bmp, int x, int y, int width, int height)
{
    FX_Draw_Mono(x, y, MONO_AND, UP_LEFT, BLACK, WHITE, bmp, width, height);
}

void ML_bmp_xor(const unsigned char *bmp, int x, int y, int width, int height)
{
    FX_Draw_Mono(x, y, MONO_XOR, UP_LEFT, BLACK, WHITE, bmp, width, height);
}

void ML_bmp_or_cl(const unsigned char *bmp, int x, int y, int width, int height)
{
    FX_Draw_Mono(x, y, MONO_OR, UP_LEFT, BLACK, WHITE, bmp, width, height);
}

void ML_bmp_and_cl(const unsigned char *bmp, int x, int y, int width, int height)
{
    FX_Draw_Mono(x, y, MONO_AND, UP_LEFT, BLACK, WHITE, bmp, width, height);
}

void ML_bmp_xor_cl(const unsigned char *bmp, int x, int y, int width, int height)
{
    FX_Draw_Mono(x, y, MONO_XOR, UP_LEFT, BLACK, WHITE, bmp, width, height);
}

void ML_bmp_8_or(const unsigned char *bmp, int x, int y)
{
    FX_Draw_Mono(x, y, MONO_OR, UP_LEFT, BLACK, WHITE, bmp, 8, 8);
}

void ML_bmp_8_and(const unsigned char *bmp, int x, int y)
{
    FX_Draw_Mono(x, y, MONO_AND, UP_LEFT, BLACK, WHITE, bmp, 8, 8);
}

void ML_bmp_8_xor(const unsigned char *bmp, int x, int y)
{
    FX_Draw_Mono(x, y, MONO_XOR, UP_LEFT, BLACK, WHITE, bmp, 8, 8);
}

void ML_bmp_8_or_cl(const unsigned char *bmp, int x, int y)
{
    FX_Draw_Mono(x, y, MONO_OR, UP_LEFT, BLACK, WHITE, bmp, 8, 8);
}

void ML_bmp_8_and_cl(const unsigned char *bmp, int x, int y)
{
    FX_Draw_Mono(x, y, MONO_AND, UP_LEFT, BLACK, WHITE, bmp, 8, 8);
}

void ML_bmp_8_xor_cl(const unsigned char *bmp, int x, int y)
{
    FX_Draw_Mono(x, y, MONO_XOR, UP_LEFT, BLACK, WHITE, bmp, 8, 8);
}

void ML_bmp_16_or(const unsigned short *bmp, int x, int y)
{
    FX_Draw_Mono(x, y, MONO_OR, UP_LEFT, BLACK, WHITE, bmp, 16, 16);
}

void ML_bmp_16_and(const unsigned short *bmp, int x, int y)
{
    FX_Draw_Mono(x, y, MONO_AND, UP_LEFT, BLACK, WHITE, bmp, 16, 16);
}

void ML_bmp_16_xor(const unsigned short *bmp, int x, int y)
{
    FX_Draw_Mono(x, y, MONO_XOR, UP_LEFT, BLACK, WHITE, bmp, 16, 16);
}

void ML_bmp_16_or_cl(const unsigned short *bmp, int x, int y)
{
    FX_Draw_Mono(x, y, MONO_OR, UP_LEFT, BLACK, WHITE, bmp, 16, 16);
}

void ML_bmp_16_and_cl(const unsigned short *bmp, int x, int y)
{
    FX_Draw_Mono(x, y, MONO_AND, UP_LEFT, BLACK, WHITE, bmp, 16, 16);
}

void ML_bmp_16_xor_cl(const unsigned short *bmp, int x, int y)
{
    FX_Draw_Mono(x, y, MONO_XOR, UP_LEFT, BLACK, WHITE, bmp, 16, 16);
}

/*****************************************************************************************/

/**************************************** 私有函数 ****************************************/

void FX_Draw_Point(int16_t x, int16_t y, uint16_t color)
{
    if (x < 0 || x > ML_SCREEN_WIDTH - 1 || y < 0 || y > ML_SCREEN_HEIGHT - 1)
        return;

    x = 2 * x + 33;
    y = 2 * y + 57;

    GE_Draw_Point(x, y, color);
    GE_Draw_Point(x + 1, y, color);
    GE_Draw_Point(x, y + 1, color);
    GE_Draw_Point(x + 1, y + 1, color);
}

uint16_t FX_Draw_GetPoint(int16_t x, int16_t y)
{
    if (x < 0 || x > ML_SCREEN_WIDTH - 1 || y < 0 || y > ML_SCREEN_HEIGHT - 1)
        return WHITE;

    return GE_Draw_GetPoint(2 * x + 33, 2 * y + 57);
}

void FX_Draw_Mono(
    int16_t x,
    int16_t y,
    uint8_t draw_mode,
    uint8_t pos_mode,
    uint16_t mono_color,
    uint16_t back_color,
    const unsigned char *pic,
    uint16_t width,
    uint16_t height)
{
    if (pos_mode == MID)
    {
        x = x - width / 2;
        y = y - height / 2;
    }

    uint8_t temp8;
    int16_t x0 = x, y0 = y;

    for (uint16_t i = 0; y - y0 < height; i++)
    {
        temp8 = pic[i];

        for (uint16_t j = 0; j < 8; j++)
        {
            if (temp8 & 0x80)
            {
                if (FX_Draw_GetPoint(x, y) == back_color && (draw_mode == MONO_OR || draw_mode == MONO_XOR || draw_mode == MONO_COVER))
                    FX_Draw_Point(x, y, mono_color);
                else if (FX_Draw_GetPoint(x, y) == mono_color && draw_mode == MONO_XOR)
                    FX_Draw_Point(x, y, back_color);
            }
            else
            {
                if (FX_Draw_GetPoint(x, y) == mono_color && (draw_mode == MONO_AND || draw_mode == MONO_COVER))
                    FX_Draw_Point(x, y, back_color);
            }

            temp8 <<= 1;

            x++;
            if (x - x0 == width)
            {
                x = x0;
                y++;
                break;
            }
        }
    }
}

/*****************************************************************************************/
