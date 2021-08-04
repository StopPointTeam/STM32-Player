//基于 HAL 库封装的 SPI 操作函数

#include "spi.h"

/**************************************** SPI2 ****************************************/
//SPI2 用于 LCD 屏幕驱动

SPI_HandleTypeDef SPI2_Handler;

/**
  * @brief  SPI2 初始化
  */
void SPI2_Init(void)
{
    SPI2_Handler.Instance = SPI2;                                          //SPI2
    SPI2_Handler.Init.Mode = SPI_MODE_MASTER;                              //设置 SPI 工作模式，设置为主模式
    SPI2_Handler.Init.Direction = SPI_DIRECTION_2LINES;                    //设置 SPI 单向或者双向的数据模式：SPI 设置为双线模式
    SPI2_Handler.Init.DataSize = SPI_DATASIZE_8BIT;                        //设置 SPI 的数据大小：SPI 发送接收 8 位帧结构
    SPI2_Handler.Init.CLKPolarity = SPI_POLARITY_HIGH;                     //串行同步时钟的空闲状态为高电平
    SPI2_Handler.Init.CLKPhase = SPI_PHASE_2EDGE;                          //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI2_Handler.Init.NSS = SPI_NSS_SOFT;                                  //NSS 信号由硬件（NSS 管脚）还是软件（使用 SSI 位）管理：内部 NSS 信号有 SSI 位控制
    SPI2_Handler.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;                    //NSS 信号脉冲失能
    SPI2_Handler.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE; //SPI 主模式 IO 状态保持使能
    SPI2_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;       //定义波特率预分频的值：波特率预分频值为 256
    SPI2_Handler.Init.FirstBit = SPI_FIRSTBIT_MSB;                         //指定数据传输从 MSB 位还是 LSB 位开始：数据传输从 MSB 位开始
    SPI2_Handler.Init.TIMode = SPI_TIMODE_DISABLE;                         //关闭 TI 模式
    SPI2_Handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;         //关闭硬件 CRC 校验
    SPI2_Handler.Init.CRCPolynomial = 7;                                   //CRC 值计算的多项式
    HAL_SPI_Init(&SPI2_Handler);

    __HAL_SPI_ENABLE(&SPI2_Handler); //使能 SPI2
    SPI2_ReadWrite8Bits(0Xff);       //启动传输
}

/**
  * @brief  SPI 底层配置，被 HAL_SPI_Init() 调用
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    RCC_PeriphCLKInitTypeDef SPI2ClkInit;

    __HAL_RCC_GPIOB_CLK_ENABLE(); //使能 GPIOB 时钟
    __HAL_RCC_SPI2_CLK_ENABLE();  //使能 SPI2 时钟

    //设置SPI2的时钟源
    SPI2ClkInit.PeriphClockSelection = RCC_PERIPHCLK_SPI2;      //设置SPI2 时钟源
    SPI2ClkInit.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL; //SPI2 时钟源使用 PLL1Q
    HAL_RCCEx_PeriphCLKConfig(&SPI2ClkInit);

    GPIO_Initure.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; //PB13 14 15
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;                        //复用推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;                            //上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;             //快速
    GPIO_Initure.Alternate = GPIO_AF5_SPI2;                     //复用为 SPI2
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);                        //GPIO 初始化
}

/**
  * @brief  SPI2 速度设置函数，
  *         SPI 速度 = PLL1Q / 分频系数，
  *         PLL1Q 一般为 200MHz
  * @param  SPI_BaudRatePrescaler: SPI_BaudRate_Prescaler SPI BaudRate Prescaler. 例如：SPI_BAUDRATEPRESCALER_4
  */
void SPI2_SetSpeed(uint32_t SPI_BaudRatePrescaler)
{
    __HAL_SPI_DISABLE(&SPI2_Handler);                     //关闭 SPI
    SPI2_Handler.Instance->CFG1 &= ~(0X7 << 28);          //位 30-28 清零，用来设置波特率
    SPI2_Handler.Instance->CFG1 |= SPI_BaudRatePrescaler; //设置 SPI 速度
    __HAL_SPI_ENABLE(&SPI2_Handler);                      //使能 SPI
}

/**
  * @brief  SPI2 读写 8 位数据
  * @param  TxData: 要写入的 8 位数据
  * @retval 读出的 8 位数据
  */
uint8_t SPI2_ReadWrite8Bits(uint8_t TxData)
{
    uint8_t Rxdata = 0xff;
    HAL_SPI_TransmitReceive(&SPI2_Handler, &TxData, &Rxdata, 1, 1000);

    return Rxdata;
}

/**
  * @brief  SPI2 读写连续数据
  * @param  pTxData: 指向写入数据的指针
  * @param  pRxData: 指向读出数据的指针
  * @param  Count: 数据长度，按 8 位划分。最大值为 0xffff (65535)
  */
void SPI2_ReadWriteBytes(uint8_t *pTxData, uint8_t *pRxData, uint32_t Count)
{
    while (Count > 0xffff)
    {
        HAL_SPI_TransmitReceive(&SPI2_Handler, pTxData, pRxData, 0xffff, 0xffff);
        Count -= 0xffff;
        pTxData += 0xffff;
        pRxData += 0xffff;
    }

    HAL_SPI_TransmitReceive(&SPI2_Handler, pTxData, pRxData, Count, 0xffff);
}

/**
  * @brief  SPI2 写 8 位数据
  * @param  Data: 要写入的 8 位数据
  */
void SPI2_Write8Bits(uint8_t Data)
{
    HAL_SPI_Transmit(&SPI2_Handler, &Data, 1, 0xffff);
}

/**
  * @brief  SPI2 写 16 位数据
  * @param  Data: 要写入的 16 位数据
  */
void SPI2_Write16Bits(uint16_t Data)
{
    uint8_t temp[2];
    temp[0] = Data >> 8;
    temp[1] = Data;

    HAL_SPI_Transmit(&SPI2_Handler, &temp, 2, 0xffff);
}

/**
  * @brief  SPI2 写 32 位数据
  * @param  Data: 要写入的 32 位数据
  */
void SPI2_Write32Bits(uint32_t Data)
{
    uint8_t temp[4];
    temp[0] = Data >> 24;
    temp[1] = Data >> 16;
    temp[2] = Data >> 8;
    temp[3] = Data;

    HAL_SPI_Transmit(&SPI2_Handler, &temp, 4, 0xffff);
}

/**
  * @brief  SPI2 写连续数据
  * @param  pData: 指向写入数据的指针
  * @param  Count: 字节数
  */
void SPI2_WriteBytes(uint8_t *pData, uint32_t Count)
{
    while (Count > 0xffff)
    {
        HAL_SPI_Transmit(&SPI2_Handler, pData, 0xffff, 0xffff);
        Count -= 0xffff;
        pData += 0xffff;
    }

    HAL_SPI_Transmit(&SPI2_Handler, pData, Count, 0xffff);
}

/**
  * @brief  SPI2 读 8 位数据
  * @retval 读出的 8 位数据
  */
uint8_t SPI2_Read8Bits(void)
{
    uint8_t data;
    HAL_SPI_Receive(&SPI2_Handler, &data, 1, 0xffff);

    return data;
}

/**
  * @brief  SPI2 读连续数据
  * @param  pData: 指向读出数据的指针
  * @param  Count: 字节数
  */
void SPI2_ReadBytes(uint8_t *pData, uint32_t Count)
{
    while (Count > 0xffff)
    {
        HAL_SPI_Receive(&SPI2_Handler, pData, 0xffff, 0xffff);
        Count -= 0xffff;
        pData += 0xffff;
    }

    HAL_SPI_Receive(&SPI2_Handler, pData, Count, 0xffff);
}

/*************************************************************************************/
