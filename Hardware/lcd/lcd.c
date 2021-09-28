//LCD �⣨ILI9431 ���ֿ⣩

#include "systick.h"
#include "spi.h"

#include "lcd.h"

/**************************************** ˽�к��� ****************************************/

/*****************************************************************************************/

/**
  * @brief  LCD ���� 8 λ����
  * @param  cmd: 8 λ����
  */
void LCD_SendCmd(uint8_t cmd)
{
    LCD_Cmd_Mode_On;

    SPI2_Write8Bits(cmd);
}

/**
  * @brief  LCD ���� 8 λ����
  * @param  data: 8 λ����
  */
void LCD_SendData8Bits(uint8_t data)
{
    LCD_Data_Mode_On;

    SPI2_Write8Bits(data);
}

/**
  * @brief  LCD ���� 16 λ����
  * @param  data: 16 λ����
  */
void LCD_SendData16Bits(uint16_t data)
{
    LCD_Data_Mode_On;

    SPI2_Write16Bits(data);
}

/**
  * @brief  LCD ���� 32 λ����
  * @param  data: 32 λ����
  */
void LCD_SendData32Bits(uint32_t data)
{
    LCD_Data_Mode_On;

    SPI2_Write32Bits(data);
}

/**
  * @brief  LCD ������������
  * @param  pData: ָ��д�����ݵ�ָ��
  * @param  Count: �ֽ���
  */
void LCD_SendDataBytes(uint8_t *pData, uint32_t Count)
{
    LCD_Data_Mode_On;

    SPI2_WriteBytes(pData, Count);
}

/**
  * @brief  LCD ���� 8 λ������� 8 λ����
  * @param  cmd: 8 λ����
  * @param  data: 8 λ����
  */
void LCD_SendCmdData8Bits(uint8_t cmd, uint8_t data)
{
    LCD_SendCmd(cmd);
    LCD_SendData8Bits(data);
}

/**
  * @brief  LCD ���� 8 λ������� 16 λ����
  * @param  cmd: 8 λ����
  * @param  data: 16 λ����
  */
void LCD_SendCmdData16Bits(uint8_t cmd, uint16_t data)
{
    LCD_SendCmd(cmd);
    LCD_SendData16Bits(data);
}

/**
  * @brief  LCD ���� 8 λ������� 32 λ����
  * @param  cmd: 8 λ����
  * @param  data: 32 λ����
  */
void LCD_SendCmdData32Bits(uint8_t cmd, uint32_t data)
{
    LCD_SendCmd(cmd);
    LCD_SendData32Bits(data);
}

/**
  * @brief  LCD ���� 8 λ�������������������
  * @param  cmd: 8 λ����
  * @param  pData: ָ��д�����ݵ�ָ��
  * @param  Count: �ֽ���
  */
void LCD_SendCmdDataBytes(uint8_t cmd, uint8_t *pData, uint32_t Count)
{
    LCD_SendCmd(cmd);
    LCD_SendDataBytes(pData, Count);
}

/**
  * @brief  LCD ���ֿ��ʼ��
  */
void LCD_Init(void)
{
    //��ʼ�� ILI9431
    //��ʼ�� PB0 PB1 PB12
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE(); //���� PB ʱ��

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_12; //PB0 PB1 PB12
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;                  //�������
    GPIO_Initure.Pull = GPIO_PULLUP;                          //����
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;           //����
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);                      //��ʼ�� PB0 PB1 PB12

    LCD_Stop_Send;
    LCD_Data_Mode_On;
    LCD_BLK_On; //��������

    SPI2_Init();                            //��ʼ�� SPI2
    SPI2_SetSpeed(SPI_BAUDRATEPRESCALER_4); //���� SPI2 �ٶ�

    LCD_Start_Send; //Ƭѡ CS λ�� 0����ʼ����

    LCD_SendCmd(LCD_CMD_SWRESET); //��λ������˯��ģʽ
    Delay_ms(50);

    //���Ŀ��� B
    uint8_t pwctrlb_param[3] = {0x00, 0xd9, 0x30};
    LCD_SendCmdDataBytes(LCD_CMD_PWCTRLB, pwctrlb_param, 3);

    LCD_SendCmdData32Bits(LCD_CMD_PWOSCTRL, 0x64031281); //��Դ���п���

    //����ʱ����� A
    uint8_t dtctrla_param[3] = {0x85, 0x10, 0x78};
    LCD_SendCmdDataBytes(LCD_CMD_DTCTRLA, dtctrla_param, 3);

    //���Ŀ��� A
    uint8_t pwctrla_param[5] = {0x39, 0x2c, 0x00, 0x34, 0x02};
    LCD_SendCmdDataBytes(LCD_CMD_PWCTRLA, pwctrla_param, 5);

    LCD_SendCmdData8Bits(LCD_CMD_PRCRTL, 0x20); //�ñȿ���

    LCD_SendCmdData16Bits(LCD_CMD_DTCTRLB, 0x0000); //����ʱ����� B

    LCD_SendCmdData8Bits(LCD_CMD_PWCTRL1, 0x21); //���Ŀ��� 1

    LCD_SendCmdData8Bits(LCD_CMD_PWCTRL2, 0x12); //���Ŀ��� 2

    LCD_SendCmdData16Bits(LCD_CMD_VMCTRL1, 0x323c); //VCOM ���� 1

    LCD_SendCmdData8Bits(LCD_CMD_VMCTRL2, 0xc1); //VCOM ���� 2

    LCD_SendCmdData8Bits(LCD_CMD_MADCTL, LCD_CMD_MADCTL_PARAM); //���������ʿ��ƣ��趨Ϊ����

    LCD_SendCmdData8Bits(LCD_CMD_PIXSET, 0x55); //���ظ�ʽ����

    LCD_SendCmdData16Bits(LCD_CMD_FRMCTR1, 0x0018); //֡���ʿ���

    LCD_SendCmdData16Bits(LCD_CMD_DISCTRL, 0x0aa2); //��ʾ���ܿ���

    LCD_SendCmdData8Bits(LCD_CMD_EN3G, 0x00); //ʹ�� 3G

    LCD_SendCmdData8Bits(LCD_CMD_GAMSET, 0x01); //٤������

    //����٤��У׼
    uint8_t pgamctrl_param[15] = {0x0f, 0x20, 0x1e, 0x09, 0x12, 0x0b, 0x50, 0xba, 0x44, 0x09, 0x14, 0x05, 0x23, 0x21, 0x00};
    LCD_SendCmdDataBytes(LCD_CMD_PGAMCTRL, pgamctrl_param, 15);

    //����٤��У׼
    uint8_t ngamctrl_param[15] = {0x00, 0x19, 0x19, 0x00, 0x12, 0x07, 0x2d, 0x28, 0x3f, 0x02, 0x0a, 0x08, 0x25, 0x2d, 0x0f};
    LCD_SendCmdDataBytes(LCD_CMD_NGAMCTRL, ngamctrl_param, 15);

    LCD_SendCmd(LCD_CMD_SLPOUT); //�˳�˯��ģʽ

    //�����˳�˯��ģʽ��ĵȴ�ʱ���ʼ���ֿ�
    uint8_t font_temp[8];
    LCD_Font_ReadAddr(font_temp, 0x00000000, 8); //�Զ� 8 �ֽ�����

    //���� ILI9431 �ֲᣬ���� LCD_CMD_SLPOUT ����ȴ� 120ms��ʵ��ȥ����ʱ���ܹ���
    // Delay_ms(120);

    LCD_Disp_On; //����ʾ

    LCD_SetWin(0, 0, LCD_WIDTH, LCD_HEIGHT);
}

/**
  * @brief  LCD ������ʾ����
  * @param  x: 0~319
  * @param  y: 0~239��x y Ϊ�������Ͻ�����
  * @param  width: ���ڿ�ȡ������ 0
  * @param  height: ���ڸ߶ȡ������ 0
  */
void LCD_SetWin(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    LCD_SendCmdData32Bits(LCD_CMD_CASET, (x << 16) | (x + width - 1));
    LCD_SendCmdData32Bits(LCD_CMD_PASET, (y << 16) | (y + height - 1));
}

/**
  * @brief  LCD �ֿ����ַ
  * @param  pData: ��������ָ��
  * @param  addr: 16 λ��ַ
  * @param  Count: �ֽ���
  */
void LCD_Font_ReadAddr(uint8_t *pData, uint32_t addr, uint16_t Count)
{
    LCD_Font_Start_Send; //CS �� 1����ʱ�ڿ����ֿ���� ILI9431

    SPI2_Write32Bits(LCD_FONT_CMD_RDADDR << 24 | addr);
    SPI2_ReadBytes(pData, Count);

    LCD_Font_Stop_Send;
}
