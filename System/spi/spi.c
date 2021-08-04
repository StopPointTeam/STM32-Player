//���� HAL ���װ�� SPI ��������

#include "spi.h"

/**************************************** SPI2 ****************************************/
//SPI2 ���� LCD ��Ļ����

SPI_HandleTypeDef SPI2_Handler;

/**
  * @brief  SPI2 ��ʼ��
  */
void SPI2_Init(void)
{
    SPI2_Handler.Instance = SPI2;                                          //SPI2
    SPI2_Handler.Init.Mode = SPI_MODE_MASTER;                              //���� SPI ����ģʽ������Ϊ��ģʽ
    SPI2_Handler.Init.Direction = SPI_DIRECTION_2LINES;                    //���� SPI �������˫�������ģʽ��SPI ����Ϊ˫��ģʽ
    SPI2_Handler.Init.DataSize = SPI_DATASIZE_8BIT;                        //���� SPI �����ݴ�С��SPI ���ͽ��� 8 λ֡�ṹ
    SPI2_Handler.Init.CLKPolarity = SPI_POLARITY_HIGH;                     //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI2_Handler.Init.CLKPhase = SPI_PHASE_2EDGE;                          //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI2_Handler.Init.NSS = SPI_NSS_SOFT;                                  //NSS �ź���Ӳ����NSS �ܽţ����������ʹ�� SSI λ�������ڲ� NSS �ź��� SSI λ����
    SPI2_Handler.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;                    //NSS �ź�����ʧ��
    SPI2_Handler.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE; //SPI ��ģʽ IO ״̬����ʹ��
    SPI2_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;       //���岨����Ԥ��Ƶ��ֵ��������Ԥ��ƵֵΪ 256
    SPI2_Handler.Init.FirstBit = SPI_FIRSTBIT_MSB;                         //ָ�����ݴ���� MSB λ���� LSB λ��ʼ�����ݴ���� MSB λ��ʼ
    SPI2_Handler.Init.TIMode = SPI_TIMODE_DISABLE;                         //�ر� TI ģʽ
    SPI2_Handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;         //�ر�Ӳ�� CRC У��
    SPI2_Handler.Init.CRCPolynomial = 7;                                   //CRC ֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI2_Handler);

    __HAL_SPI_ENABLE(&SPI2_Handler); //ʹ�� SPI2
    SPI2_ReadWrite8Bits(0Xff);       //��������
}

/**
  * @brief  SPI �ײ����ã��� HAL_SPI_Init() ����
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    RCC_PeriphCLKInitTypeDef SPI2ClkInit;

    __HAL_RCC_GPIOB_CLK_ENABLE(); //ʹ�� GPIOB ʱ��
    __HAL_RCC_SPI2_CLK_ENABLE();  //ʹ�� SPI2 ʱ��

    //����SPI2��ʱ��Դ
    SPI2ClkInit.PeriphClockSelection = RCC_PERIPHCLK_SPI2;      //����SPI2 ʱ��Դ
    SPI2ClkInit.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL; //SPI2 ʱ��Դʹ�� PLL1Q
    HAL_RCCEx_PeriphCLKConfig(&SPI2ClkInit);

    GPIO_Initure.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; //PB13 14 15
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;                        //�����������
    GPIO_Initure.Pull = GPIO_PULLUP;                            //����
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;             //����
    GPIO_Initure.Alternate = GPIO_AF5_SPI2;                     //����Ϊ SPI2
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);                        //GPIO ��ʼ��
}

/**
  * @brief  SPI2 �ٶ����ú�����
  *         SPI �ٶ� = PLL1Q / ��Ƶϵ����
  *         PLL1Q һ��Ϊ 200MHz
  * @param  SPI_BaudRatePrescaler: SPI_BaudRate_Prescaler SPI BaudRate Prescaler. ���磺SPI_BAUDRATEPRESCALER_4
  */
void SPI2_SetSpeed(uint32_t SPI_BaudRatePrescaler)
{
    __HAL_SPI_DISABLE(&SPI2_Handler);                     //�ر� SPI
    SPI2_Handler.Instance->CFG1 &= ~(0X7 << 28);          //λ 30-28 ���㣬�������ò�����
    SPI2_Handler.Instance->CFG1 |= SPI_BaudRatePrescaler; //���� SPI �ٶ�
    __HAL_SPI_ENABLE(&SPI2_Handler);                      //ʹ�� SPI
}

/**
  * @brief  SPI2 ��д 8 λ����
  * @param  TxData: Ҫд��� 8 λ����
  * @retval ������ 8 λ����
  */
uint8_t SPI2_ReadWrite8Bits(uint8_t TxData)
{
    uint8_t Rxdata = 0xff;
    HAL_SPI_TransmitReceive(&SPI2_Handler, &TxData, &Rxdata, 1, 1000);

    return Rxdata;
}

/**
  * @brief  SPI2 ��д��������
  * @param  pTxData: ָ��д�����ݵ�ָ��
  * @param  pRxData: ָ��������ݵ�ָ��
  * @param  Count: ���ݳ��ȣ��� 8 λ���֡����ֵΪ 0xffff (65535)
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
  * @brief  SPI2 д 8 λ����
  * @param  Data: Ҫд��� 8 λ����
  */
void SPI2_Write8Bits(uint8_t Data)
{
    HAL_SPI_Transmit(&SPI2_Handler, &Data, 1, 0xffff);
}

/**
  * @brief  SPI2 д 16 λ����
  * @param  Data: Ҫд��� 16 λ����
  */
void SPI2_Write16Bits(uint16_t Data)
{
    uint8_t temp[2];
    temp[0] = Data >> 8;
    temp[1] = Data;

    HAL_SPI_Transmit(&SPI2_Handler, &temp, 2, 0xffff);
}

/**
  * @brief  SPI2 д 32 λ����
  * @param  Data: Ҫд��� 32 λ����
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
  * @brief  SPI2 д��������
  * @param  pData: ָ��д�����ݵ�ָ��
  * @param  Count: �ֽ���
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
  * @brief  SPI2 �� 8 λ����
  * @retval ������ 8 λ����
  */
uint8_t SPI2_Read8Bits(void)
{
    uint8_t data;
    HAL_SPI_Receive(&SPI2_Handler, &data, 1, 0xffff);

    return data;
}

/**
  * @brief  SPI2 ����������
  * @param  pData: ָ��������ݵ�ָ��
  * @param  Count: �ֽ���
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
