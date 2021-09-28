//ADC ��

#include "sys.h"
#include "tim.h"

#include "adc.h"

//32 �ֽڶ���
ALIGN_32BYTES(uint16_t __attribute__((section(".RAM_D3"))) ADCxValues[2]);

/**
  * @brief  ADC ��ʼ������
  */
void ADC_Init(void)
{
    ADC_HandleTypeDef AdcHandle = {0};
    DMA_HandleTypeDef DMA_Handle = {0};
    ADC_ChannelConfTypeDef sConfig = {0};
    GPIO_InitTypeDef GPIO_InitStruct;

    //����ʱ��
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL2.PLL2M = 25;
    PeriphClkInitStruct.PLL2.PLL2N = 504;
    PeriphClkInitStruct.PLL2.PLL2P = 7;
    PeriphClkInitStruct.PLL2.PLL2Q = 7;
    PeriphClkInitStruct.PLL2.PLL2R = 7;
    PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
    PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    //GPIO
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    //DMA
    __HAL_RCC_DMA1_CLK_ENABLE();
    DMA_Handle.Instance = DMA1_Stream1;                            //ʹ�õ�DMA1 Stream1
    DMA_Handle.Init.Request = DMA_REQUEST_ADC3;                    //�������Ͳ��� DMA_REQUEST_ADC3
    DMA_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;              //���䷽���ǴӴ洢��������
    DMA_Handle.Init.PeriphInc = DMA_PINC_DISABLE;                  //�����ַ������ֹ
    DMA_Handle.Init.MemInc = DMA_MINC_ENABLE;                      //�洢����ַ����ʹ��
    DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; //�������ݴ���λ��ѡ����֣��� 16bit
    DMA_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    //�洢�����ݴ���λ��ѡ����֣��� 16bit
    DMA_Handle.Init.Mode = DMA_CIRCULAR;                           //ѭ��ģʽ
    DMA_Handle.Init.Priority = DMA_PRIORITY_LOW;                   //���ȼ���
    DMA_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;               //��ֹ FIFO
    DMA_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    DMA_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
    DMA_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;

    if (HAL_DMA_Init(&DMA_Handle) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    //���� ADC ����� DMA ���
    __HAL_LINKDMA(&AdcHandle, DMA_Handle, DMA_Handle);

    //���� ADC
    __HAL_RCC_ADC3_CLK_ENABLE();
    AdcHandle.Instance = ADC3;
    AdcHandle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV8;                      //����PLL�첽ʱ�ӣ�8��Ƶ���� 72MHz / 8 = 36MHz
    AdcHandle.Init.Resolution = ADC_RESOLUTION_16B;                            //16 λ�ֱ���
    AdcHandle.Init.ScanConvMode = ADC_SCAN_ENABLE;                             //����ɨ��
    AdcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;                         //EOC ת��������־
    AdcHandle.Init.LowPowerAutoWait = DISABLE;                                 //��ֹ�͹����Զ��ӳ�����
    AdcHandle.Init.ContinuousConvMode = ENABLE;                                //�����Զ�ת��
    AdcHandle.Init.NbrOfConversion = 2;                                        //ʹ���� 2 ��ת��ͨ��
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;                            //��ֹ������ģʽ
    AdcHandle.Init.NbrOfDiscConversion = 1;                                    //��ֹ������ģʽ�󣬴˲������ԣ���λ���������ò�����������ͨ����
    AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;                      //�����������
    AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;     //������������Ļ�����λ����
    AdcHandle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR; //DMA ѭ��ģʽ���� ADC ת��������
    AdcHandle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;                         //ADC ת������Ļ������� ADC �����ݼĴ���
    AdcHandle.Init.OversamplingMode = DISABLE;                                 //��ֹ������

    //��ʼ�� ADC
    if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    //У׼ ADC������ƫ��У׼
    if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    //���� 1������ PC0 ����
    sConfig.Channel = ADC_CHANNEL_10;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5; //��������
    sConfig.SingleDiff = ADC_SINGLE_ENDED;             //��������
    sConfig.OffsetNumber = ADC_OFFSET_NONE;            //��ƫ��
    sConfig.Offset = 0;
    sConfig.OffsetRightShift = DISABLE;       //��ֹ����
    sConfig.OffsetSignedSaturation = DISABLE; //��ֹ�з��ű���

    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    //���� 2������ PC1 ����
    sConfig.Channel = ADC_CHANNEL_11;
    sConfig.Rank = ADC_REGULAR_RANK_2;
    sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    sConfig.OffsetRightShift = DISABLE;
    sConfig.OffsetSignedSaturation = DISABLE;

    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    //���� ADC �� DMA ��ʽ����
    if (HAL_ADC_Start_DMA(&AdcHandle, (uint32_t *)ADCxValues, 2) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    //���ö�ʱ��
    TIM_Set(TIM3, 1000, 0, 0);
}

/**
  * @brief  ��ȡ PC0 ��ѹ
  * @retval ��ѹ����λ V
  */
float ADC_Get_PC0(void)
{
    return ADCxValues[0] * 3.3 / 65536;
}

/**
  * @brief  ��ȡ PC1 ��ѹ
  * @retval ��ѹ����λ V
  */
float ADC_Get_PC1(void)
{
    return ADCxValues[1] * 3.3 / 65536;
}

/**
  * @brief  TIM3 �жϴ��� 
  */
void TIM3_IRQHandler(void)
{
    if ((TIM3->SR & TIM_FLAG_UPDATE) != RESET)
    {
        //������±�־
        TIM3->SR = ~TIM_FLAG_UPDATE;

        SCB_InvalidateDCache_by_Addr((uint32_t *)ADCxValues, sizeof(uint32_t));
    }
}
