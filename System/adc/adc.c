//ADC 库

#include "sys.h"
#include "tim.h"

#include "adc.h"

//32 字节对齐
ALIGN_32BYTES(uint16_t __attribute__((section(".RAM_D3"))) ADCxValues[2]);

/**
  * @brief  ADC 初始化函数
  */
void ADC_Init(void)
{
    ADC_HandleTypeDef AdcHandle = {0};
    DMA_HandleTypeDef DMA_Handle = {0};
    ADC_ChannelConfTypeDef sConfig = {0};
    GPIO_InitTypeDef GPIO_InitStruct;

    //配置时钟
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
    DMA_Handle.Instance = DMA1_Stream1;                            //使用的DMA1 Stream1
    DMA_Handle.Init.Request = DMA_REQUEST_ADC3;                    //请求类型采用 DMA_REQUEST_ADC3
    DMA_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;              //传输方向是从存储器到外设
    DMA_Handle.Init.PeriphInc = DMA_PINC_DISABLE;                  //外设地址自增禁止
    DMA_Handle.Init.MemInc = DMA_MINC_ENABLE;                      //存储器地址自增使能
    DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; //外设数据传输位宽选择半字，即 16bit
    DMA_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    //存储器数据传输位宽选择半字，即 16bit
    DMA_Handle.Init.Mode = DMA_CIRCULAR;                           //循环模式
    DMA_Handle.Init.Priority = DMA_PRIORITY_LOW;                   //优先级低
    DMA_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;               //禁止 FIFO
    DMA_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    DMA_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
    DMA_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;

    if (HAL_DMA_Init(&DMA_Handle) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    //关联 ADC 句柄和 DMA 句柄
    __HAL_LINKDMA(&AdcHandle, DMA_Handle, DMA_Handle);

    //配置 ADC
    __HAL_RCC_ADC3_CLK_ENABLE();
    AdcHandle.Instance = ADC3;
    AdcHandle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV8;                      //采用PLL异步时钟，8分频，即 72MHz / 8 = 36MHz
    AdcHandle.Init.Resolution = ADC_RESOLUTION_16B;                            //16 位分辨率
    AdcHandle.Init.ScanConvMode = ADC_SCAN_ENABLE;                             //开启扫描
    AdcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;                         //EOC 转换结束标志
    AdcHandle.Init.LowPowerAutoWait = DISABLE;                                 //禁止低功耗自动延迟特性
    AdcHandle.Init.ContinuousConvMode = ENABLE;                                //启用自动转换
    AdcHandle.Init.NbrOfConversion = 2;                                        //使用了 2 个转换通道
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;                            //禁止不连续模式
    AdcHandle.Init.NbrOfDiscConversion = 1;                                    //禁止不连续模式后，此参数忽略，此位是用来配置不连续子组中通道数
    AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;                      //采用软件触发
    AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;     //采用软件触发的话，此位忽略
    AdcHandle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR; //DMA 循环模式接收 ADC 转换的数据
    AdcHandle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;                         //ADC 转换溢出的话，覆盖 ADC 的数据寄存器
    AdcHandle.Init.OversamplingMode = DISABLE;                                 //禁止过采样

    //初始化 ADC
    if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    //校准 ADC，采用偏移校准
    if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    //序列 1，采样 PC0 引脚
    sConfig.Channel = ADC_CHANNEL_10;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5; //采样周期
    sConfig.SingleDiff = ADC_SINGLE_ENDED;             //单端输入
    sConfig.OffsetNumber = ADC_OFFSET_NONE;            //无偏移
    sConfig.Offset = 0;
    sConfig.OffsetRightShift = DISABLE;       //禁止右移
    sConfig.OffsetSignedSaturation = DISABLE; //禁止有符号饱和

    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    //序列 2，采样 PC1 引脚
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

    //启动 ADC 的 DMA 方式传输
    if (HAL_ADC_Start_DMA(&AdcHandle, (uint32_t *)ADCxValues, 2) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    //设置定时器
    TIM_Set(TIM3, 1000, 0, 0);
}

/**
  * @brief  获取 PC0 电压
  * @retval 电压，单位 V
  */
float ADC_Get_PC0(void)
{
    return ADCxValues[0] * 3.3 / 65536;
}

/**
  * @brief  获取 PC1 电压
  * @retval 电压，单位 V
  */
float ADC_Get_PC1(void)
{
    return ADCxValues[1] * 3.3 / 65536;
}

/**
  * @brief  TIM3 中断处理 
  */
void TIM3_IRQHandler(void)
{
    if ((TIM3->SR & TIM_FLAG_UPDATE) != RESET)
    {
        //清除更新标志
        TIM3->SR = ~TIM_FLAG_UPDATE;

        SCB_InvalidateDCache_by_Addr((uint32_t *)ADCxValues, sizeof(uint32_t));
    }
}
