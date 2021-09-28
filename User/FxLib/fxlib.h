//fxlib 兼容层，用于从 fx-9860 平台向 STM32-Player 移植程序

#ifndef __FXLIB_H
#define __FXLIB_H

#define ML_SCREEN_WIDTH 128
#define ML_SCREEN_HEIGHT 64

void Sleep(int millisecond);

void ML_clear_vram();
void ML_clear_screen();
void ML_display_vram();

void ML_bmp_or(const unsigned char *bmp, int x, int y, int width, int height);
void ML_bmp_and(const unsigned char *bmp, int x, int y, int width, int height);
void ML_bmp_xor(const unsigned char *bmp, int x, int y, int width, int height);
void ML_bmp_or_cl(const unsigned char *bmp, int x, int y, int width, int height);
void ML_bmp_and_cl(const unsigned char *bmp, int x, int y, int width, int height);
void ML_bmp_xor_cl(const unsigned char *bmp, int x, int y, int width, int height);

void ML_bmp_8_or(const unsigned char *bmp, int x, int y);
void ML_bmp_8_and(const unsigned char *bmp, int x, int y);
void ML_bmp_8_xor(const unsigned char *bmp, int x, int y);
void ML_bmp_8_or_cl(const unsigned char *bmp, int x, int y);
void ML_bmp_8_and_cl(const unsigned char *bmp, int x, int y);
void ML_bmp_8_xor_cl(const unsigned char *bmp, int x, int y);

void ML_bmp_16_or(const unsigned short *bmp, int x, int y);
void ML_bmp_16_and(const unsigned short *bmp, int x, int y);
void ML_bmp_16_xor(const unsigned short *bmp, int x, int y);
void ML_bmp_16_or_cl(const unsigned short *bmp, int x, int y);
void ML_bmp_16_and_cl(const unsigned short *bmp, int x, int y);
void ML_bmp_16_xor_cl(const unsigned short *bmp, int x, int y);

#endif
