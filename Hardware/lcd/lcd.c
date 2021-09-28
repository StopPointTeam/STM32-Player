//LCD 库（ILI9431 及字库）

#include "systick.h"
#include "spi.h"

#include "lcd.h"

/**************************************** 私有函数 ****************************************/

/*****************************************************************************************/

/**
  * @brief  LCD 发送 8 位命令
  * @param  cmd: 8 位命令
  */
void LCD_SendCmd(uint8_t cmd)
{
    LCD_Cmd_Mode_On;

    SPI2_Write8Bits(cmd);
}

/**
  * @brief  LCD 发送 8 位数据
  * @param  data: 8 位数据
  */
void LCD_SendData8Bits(uint8_t data)
{
    LCD_Data_Mode_On;

    SPI2_Write8Bits(data);
}

/**
  * @brief  LCD 发送 16 位数据
  * @param  data: 16 位数据
  */
void LCD_SendData16Bits(uint16_t data)
{
    LCD_Data_Mode_On;

    SPI2_Write16Bits(data);
}

/**
  * @brief  LCD 发送 32 位数据
  * @param  data: 32 位数据
  */
void LCD_SendData32Bits(uint32_t data)
{
    LCD_Data_Mode_On;

    SPI2_Write32Bits(data);
}

/**
  * @brief  LCD 发送连续数据
  * @param  pData: 指向写入数据的指针
  * @param  Count: 字节数
  */
void LCD_SendDataBytes(uint8_t *pData, uint32_t Count)
{
    LCD_Data_Mode_On;

    SPI2_WriteBytes(pData, Count);
}

/**
  * @brief  LCD 发送 8 位命令并发送 8 位数据
  * @param  cmd: 8 位命令
  * @param  data: 8 位数据
  */
void LCD_SendCmdData8Bits(uint8_t cmd, uint8_t data)
{
    LCD_SendCmd(cmd);
    LCD_SendData8Bits(data);
}

/**
  * @brief  LCD 发送 8 位命令并发送 16 位数据
  * @param  cmd: 8 位命令
  * @param  data: 16 位数据
  */
void LCD_SendCmdData16Bits(uint8_t cmd, uint16_t data)
{
    LCD_SendCmd(cmd);
    LCD_SendData16Bits(data);
}

/**
  * @brief  LCD 发送 8 位命令并发送 32 位数据
  * @param  cmd: 8 位命令
  * @param  data: 32 位数据
  */
void LCD_SendCmdData32Bits(uint8_t cmd, uint32_t data)
{
    LCD_SendCmd(cmd);
    LCD_SendData32Bits(data);
}

/**
  * @brief  LCD 发送 8 位命令并发送连续数据数据
  * @param  cmd: 8 位命令
  * @param  pData: 指向写入数据的指针
  * @param  Count: 字节数
  */
void LCD_SendCmdDataBytes(uint8_t cmd, uint8_t *pData, uint32_t Count)
{
    LCD_SendCmd(cmd);
    LCD_SendDataBytes(pData, Count);
}

/**
  * @brief  LCD 及字库初始化
  */
void LCD_Init(void)
{
    //初始化 ILI9431
    //初始化 PB0 PB1 PB12
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE(); //开启 PB 时钟

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_12; //PB0 PB1 PB12
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;                  //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;                          //上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;           //高速
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);                      //初始化 PB0 PB1 PB12

    LCD_Stop_Send;
    LCD_Data_Mode_On;
    LCD_BLK_On; //点亮背光

    SPI2_Init();                            //初始化 SPI2
    SPI2_SetSpeed(SPI_BAUDRATEPRESCALER_4); //设置 SPI2 速度

    LCD_Start_Send; //片选 CS 位置 0，开始传输

    LCD_SendCmd(LCD_CMD_SWRESET); //软复位，进入睡眠模式
    Delay_ms(50);

    //功耗控制 B
    uint8_t pwctrlb_param[3] = {0x00, 0xd9, 0x30};
    LCD_SendCmdDataBytes(LCD_CMD_PWCTRLB, pwctrlb_param, 3);

    LCD_SendCmdData32Bits(LCD_CMD_PWOSCTRL, 0x64031281); //电源序列控制

    //驱动时序控制 A
    uint8_t dtctrla_param[3] = {0x85, 0x10, 0x78};
    LCD_SendCmdDataBytes(LCD_CMD_DTCTRLA, dtctrla_param, 3);

    //功耗控制 A
    uint8_t pwctrla_param[5] = {0x39, 0x2c, 0x00, 0x34, 0x02};
    LCD_SendCmdDataBytes(LCD_CMD_PWCTRLA, pwctrla_param, 5);

    LCD_SendCmdData8Bits(LCD_CMD_PRCRTL, 0x20); //泵比控制

    LCD_SendCmdData16Bits(LCD_CMD_DTCTRLB, 0x0000); //驱动时序控制 B

    LCD_SendCmdData8Bits(LCD_CMD_PWCTRL1, 0x21); //功耗控制 1

    LCD_SendCmdData8Bits(LCD_CMD_PWCTRL2, 0x12); //功耗控制 2

    LCD_SendCmdData16Bits(LCD_CMD_VMCTRL1, 0x323c); //VCOM 控制 1

    LCD_SendCmdData8Bits(LCD_CMD_VMCTRL2, 0xc1); //VCOM 控制 2

    LCD_SendCmdData8Bits(LCD_CMD_MADCTL, LCD_CMD_MADCTL_PARAM); //储存器访问控制，设定为横屏

    LCD_SendCmdData8Bits(LCD_CMD_PIXSET, 0x55); //像素格式设置

    LCD_SendCmdData16Bits(LCD_CMD_FRMCTR1, 0x0018); //帧速率控制

    LCD_SendCmdData16Bits(LCD_CMD_DISCTRL, 0x0aa2); //显示功能控制

    LCD_SendCmdData8Bits(LCD_CMD_EN3G, 0x00); //使能 3G

    LCD_SendCmdData8Bits(LCD_CMD_GAMSET, 0x01); //伽马设置

    //正极伽马校准
    uint8_t pgamctrl_param[15] = {0x0f, 0x20, 0x1e, 0x09, 0x12, 0x0b, 0x50, 0xba, 0x44, 0x09, 0x14, 0x05, 0x23, 0x21, 0x00};
    LCD_SendCmdDataBytes(LCD_CMD_PGAMCTRL, pgamctrl_param, 15);

    //负极伽马校准
    uint8_t ngamctrl_param[15] = {0x00, 0x19, 0x19, 0x00, 0x12, 0x07, 0x2d, 0x28, 0x3f, 0x02, 0x0a, 0x08, 0x25, 0x2d, 0x0f};
    LCD_SendCmdDataBytes(LCD_CMD_NGAMCTRL, ngamctrl_param, 15);

    LCD_SendCmd(LCD_CMD_SLPOUT); //退出睡眠模式

    //利用退出睡眠模式后的等待时间初始化字库
    uint8_t font_temp[8];
    LCD_Font_ReadAddr(font_temp, 0x00000000, 8); //试读 8 字节数据

    //根据 ILI9431 手册，发送 LCD_CMD_SLPOUT 后需等待 120ms。实测去除延时仍能工作
    // Delay_ms(120);

    LCD_Disp_On; //开显示

    LCD_SetWin(0, 0, LCD_WIDTH, LCD_HEIGHT);
}

/**
  * @brief  LCD 设置显示窗口
  * @param  x: 0~319
  * @param  y: 0~239。x y 为窗口左上角坐标
  * @param  width: 窗口宽度。需大于 0
  * @param  height: 窗口高度。需大于 0
  */
void LCD_SetWin(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    LCD_SendCmdData32Bits(LCD_CMD_CASET, (x << 16) | (x + width - 1));
    LCD_SendCmdData32Bits(LCD_CMD_PASET, (y << 16) | (y + height - 1));
}

/**
  * @brief  LCD 字库读地址
  * @param  pData: 读出内容指针
  * @param  addr: 16 位地址
  * @param  Count: 字节数
  */
void LCD_Font_ReadAddr(uint8_t *pData, uint32_t addr, uint16_t Count)
{
    LCD_Font_Start_Send; //CS 置 1，此时在控制字库而非 ILI9431

    SPI2_Write32Bits(LCD_FONT_CMD_RDADDR << 24 | addr);
    SPI2_ReadBytes(pData, Count);

    LCD_Font_Stop_Send;
}
