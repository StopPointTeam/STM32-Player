//LCD 库（ILI9431 及字库）

#ifndef __LCD_H
#define __LCD_H

#include "sys.h"

/**************************************** 屏幕参数 ****************************************/
//采用横屏
#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define LCD_CMD_MADCTL_PARAM 0x68 //储存器访问控制命令参数。若更改为竖屏需要修改此参数

/*****************************************************************************************/

/**************************************** 字库位置 ****************************************/
#define ADDR_ASCII12 0x00080000
#define ADDR_ASCII16 0x00080800
#define ADDR_ASCII24 0x00081200
#define ADDR_ASCII32 0x00082600
#define ADDR_ASCII48 0x00084800
#define ADDR_ASCII64 0x00089200

#define ADDR_GBK12 0x00091400
#define ADDR_GBK16 0x0011dd00
#define ADDR_GBK24 0x001da000
#define ADDR_GBK32 0x00380000

/***************************************** 位控制 *****************************************/
//CS 位控制
#define LCD_CS_Set HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)
#define LCD_CS_Clr HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)

//RS 位控制（D/C）
#define LCD_RS_Set HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)
#define LCD_RS_Clr HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)

//BLK 位控制
#define LCD_BLK_Set HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)
#define LCD_BLK_Clr HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)

/*****************************************************************************************/

/****************************************** 命令 ******************************************/
//LCD 命令，即 ILI9341 命令
#define LCD_CMD_NOP 0x00
#define LCD_CMD_SWRESET 0x01
#define LCD_CMD_RDDIDIF 0x04
#define LCD_CMD_RDDST 0x09
#define LCD_CMD_RDDPM 0x0a
#define LCD_CMD_RDDMADCTL 0x0b
#define LCD_CMD_RDDCOLMOD 0x0c
#define LCD_CMD_RDDIM 0x0d
#define LCD_CMD_RDDSM 0x0e
#define LCD_CMD_RDDSDR 0x0f
#define LCD_CMD_SPLIN 0x10
#define LCD_CMD_SLPOUT 0x11
#define LCD_CMD_PTLON 0x12
#define LCD_CMD_NORON 0x13
#define LCD_CMD_DINVOFF 0x20
#define LCD_CMD_DINVON 0x21
#define LCD_CMD_GAMSET 0x26
#define LCD_CMD_DISPOFF 0x28
#define LCD_CMD_DISPON 0x29
#define LCD_CMD_CASET 0x2a
#define LCD_CMD_PASET 0x2b
#define LCD_CMD_RAMWR 0x2c
#define LCD_CMD_RGBSET 0x2d
#define LCD_CMD_RAMRD 0x2e
#define LCD_CMD_PLTAR 0x30
#define LCD_CMD_VSCRDEF 0x33
#define LCD_CMD_TEOFF 0x34
#define LCD_CMD_TEON 0x35
#define LCD_CMD_MADCTL 0x36
#define LCD_CMD_VSCRSADD 0x37
#define LCD_CMD_IDMOFF 0x38
#define LCD_CMD_IDMON 0x39
#define LCD_CMD_PIXSET 0x3a
#define LCD_CMD_WRITE_MEMORY_CONTINUE 0x3c
#define LCD_CMD_READ_MEMORY_CONTINUE 0x3e
#define LCD_CMD_SET_TEAR_SCANLINE 0x44
#define LCD_CMD_GET_SCANLINE 0x45
#define LCD_CMD_WRDISBV 0x51
#define LCD_CMD_RDDISBV 0x52
#define LCD_CMD_WRCTRLD 0x53
#define LCD_CMD_RDCTRLD 0x54
#define LCD_CMD_WRCABC 0x55
#define LCD_CMD_RDCABC 0x56
#define LCD_CMD_WRCABCMB 0x5e
#define LCD_CMD_RDCABCMB 0x5f
#define LCD_CMD_RDID1 0xda
#define LCD_CMD_RDID2 0xdb
#define LCD_CMD_RDID3 0xdc
#define LCD_CMD_IFMODE 0xb0
#define LCD_CMD_FRMCTR1 0xb1
#define LCD_CMD_FRMCTR2 0xb2
#define LCD_CMD_FRMCTR3 0xb3
#define LCD_CMD_INVTR 0xb4
#define LCD_CMD_PRCTR 0xb5
#define LCD_CMD_DISCTRL 0xb6
#define LCD_CMD_ETMOD 0xb7
#define LCD_CMD_BACKLIGHTCTRL1 0xb8
#define LCD_CMD_BACKLIGHTCTRL2 0xb9
#define LCD_CMD_BACKLIGHTCTRL3 0xba
#define LCD_CMD_BACKLIGHTCTRL4 0xbb
#define LCD_CMD_BACKLIGHTCTRL5 0xbc
#define LCD_CMD_BACKLIGHTCTRL7 0xbe
#define LCD_CMD_BACKLIGHTCTRL8 0xbf
#define LCD_CMD_PWCTRL1 0xc0
#define LCD_CMD_PWCTRL2 0xc1
#define LCD_CMD_VMCTRL1 0xc5
#define LCD_CMD_VMCTRL2 0xc7
#define LCD_CMD_NVMWR 0xd0
#define LCD_CMD_NVMPKEY 0xd1
#define LCD_CMD_RDNVM 0xd2
#define LCD_CMD_RDID4 0xd3
#define LCD_CMD_PGAMCTRL 0xe0
#define LCD_CMD_NGAMCTRL 0xe1
#define LCD_CMD_DGAMCTRL1 0xe2
#define LCD_CMD_DGAMCTRL2 0xe3
#define LCD_CMD_IFCTL 0xf6
#define LCD_CMD_PWCTRLA 0xcb
#define LCD_CMD_PWCTRLB 0xcf
#define LCD_CMD_DTCTRLA 0xe8
#define LCD_CMD_DTCTRLB 0xea
#define LCD_CMD_PWOSCTRL 0xed
#define LCD_CMD_EN3G 0xf2
#define LCD_CMD_PRCRTL 0xf7

//字库命令
#define LCD_FONT_CMD_RDADDR 0x03

/*****************************************************************************************/

/***************************************** 位切换宏 ***************************************/
//ILI9431
#define LCD_Start_Send LCD_CS_Clr //进入 LCD 发送模式（CS 置 0，此时操作 ILI9431）
#define LCD_Stop_Send LCD_CS_Set  //退出 LCD 发送模式（CS 置 1）

#define LCD_Cmd_Mode_On LCD_RS_Clr  //进入命令模式（RS 置 0）
#define LCD_Data_Mode_On LCD_RS_Set //进入数据模式（RS 置 1）

#define LCD_BLK_On LCD_BLK_Set  //开背光
#define LCD_BLK_Off LCD_BLK_Clr //关背光

//字库
#define LCD_Font_Start_Send LCD_CS_Set //进入 LCD Font 发送模式（CS 置 1，此时操作字库）
#define LCD_Font_Stop_Send LCD_CS_Clr  //退出 LCD Font 发送模式（CS 置 0）

/*****************************************************************************************/

/***************************************** 命令宏 ****************************************/
#define LCD_Disp_On LCD_SendCmd(LCD_CMD_DISPON)   //开显示
#define LCD_Disp_Off LCD_SendCmd(LCD_CMD_DISPOFF) //冻结显示。可用于关闭从显存到屏幕的刷新，防止因刷屏速度慢导致波纹出现

#define LCD_Reverse_On LCD_SendCmd(LCD_CMD_DINVON)   //开全局反色模式
#define LCD_Reverse_Off LCD_SendCmd(LCD_CMD_DINVOFF) //关全局反色模式

#define LCD_RAM_Wr LCD_SendCmd(LCD_CMD_RAMWR) //开始写显存
#define LCD_RAM_Rd LCD_SendCmd(LCD_CMD_RAMRD) //开始读显存

/**************************************** 通讯函数 ****************************************/

void LCD_SendCmd(uint8_t cmd);
void LCD_SendData8Bits(uint8_t data);
void LCD_SendData16Bits(uint16_t data);
void LCD_SendData32Bits(uint32_t data);
void LCD_SendDataBytes(uint8_t *pData, uint32_t Count);
void LCD_SendCmdData8Bits(uint8_t cmd, uint8_t data);
void LCD_SendCmdData16Bits(uint8_t cmd, uint16_t data);
void LCD_SendCmdData32Bits(uint8_t cmd, uint32_t data);
void LCD_SendCmdDataBytes(uint8_t cmd, uint8_t *pData, uint32_t Count);

/*****************************************************************************************/

/**************************************** 操作函数 ****************************************/

void LCD_Init(void);
void LCD_SetWin(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

void LCD_Font_ReadAddr(uint8_t *pData, uint32_t addr, uint16_t Count);

#endif
