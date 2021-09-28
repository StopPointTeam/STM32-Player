//系统常用调用

#include "sys.h"

/**
  * @brief  配置 MPU
  */
void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;

    /* 禁止 MPU */
    HAL_MPU_Disable();

    /* 配置AXI SRAM的MPU属性为Write through, read allocate，no write allocate */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x24000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* 配置FMC扩展IO的MPU属性为Device或者Strongly Ordered */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x60000000;
    MPU_InitStruct.Size = ARM_MPU_REGION_SIZE_64KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE; /* 不能用MPU_ACCESS_CACHEABLE;会出现2次CS、WE信号 */
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* 配置SRAM4的属性为Write through, read allocate，no write allocate */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x38000000;
    MPU_InitStruct.Size = ARM_MPU_REGION_SIZE_64KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER2;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /*使能 MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  使能 L1-Cache
  */
void Cache_Enable(void)
{
    SCB_EnableICache();  //使能 I-Cache
    SCB_EnableDCache();  //使能 D-Cache
    SCB->CACR |= 1 << 2; //强制 D-Cache透写
}

/**
  * @brief  初始化系统时钟
  */
void SystemClock_Init(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    HAL_StatusTypeDef ret = HAL_OK;

    //锁住SCU (Supply configuration update)
    MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
    {
    }

    //使能 HSE，并选择 HSE 作为 PLL 时钟源
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
    RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

    RCC_OscInitStruct.PLL.PLLM = 5;
    RCC_OscInitStruct.PLL.PLLN = 160;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLQ = 4;

    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /* 
       选择 PLL 的输出作为系统时钟
       配置 RCC_CLOCKTYPE_SYSCLK 系统时钟
       配置 RCC_CLOCKTYPE_HCLK 时钟，对应 AHB1 AHB2 AHB3 AHB4 总线
       配置 RCC_CLOCKTYPE_PCLK1 时钟，对应 APB1 总线
       配置 RCC_CLOCKTYPE_PCLK2 时钟，对应 APB2 总线
       配置 RCC_CLOCKTYPE_D1PCLK1 时钟，对应 APB3 总线
       配置 RCC_CLOCKTYPE_D3PCLK1 时钟，对应 APB4 总线     
    */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 |
                                   RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1);

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);

    __HAL_RCC_CSI_ENABLE();

    __HAL_RCC_SYSCFG_CLK_ENABLE();

    HAL_EnableCompensationCell();

    __HAL_RCC_D2SRAM1_CLK_ENABLE();
    __HAL_RCC_D2SRAM2_CLK_ENABLE();
    __HAL_RCC_D2SRAM3_CLK_ENABLE();
}

/**
  * @brief  判断 I_Cache 是否打开
  * @retval 关闭返回 0，打开返回 1
  */
uint8_t Get_ICacheSta(void)
{
    uint8_t sta;
    sta = ((SCB->CCR) >> 17) & 0X01;
    return sta;
}

/**
  * @brief  判断 I_Dache 是否打开
  * @retval 关闭返回 0，打开返回 1
  */
uint8_t Get_DCacheSta(void)
{
    uint8_t sta;
    sta = ((SCB->CCR) >> 16) & 0X01;
    return sta;
}

void Error_Handler(char *file, uint32_t line)
{
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    if (line == 0)
        return;

    while (1)
        ;
}
