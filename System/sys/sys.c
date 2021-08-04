//系统初始化

#include "sys.h"

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
  * @param  plln: PLL1 倍频系数（PLL倍频），取值范围：4~512
  * @param  pllm: PLL1 预分频系数（进PLL之前的分频），取值范围：2~63
  * @param  pllp: PLL1 的 p 分频系数（PLL之后的分频），分频后作为系统时钟，取值范围：2~128（且必须是 2 的倍数）
  * @param  pllq: PLL1 的 q 分频系数（PLL之后的分频），取值范围：1~128.
  * @retval 成功返回 0，失败返回 1
  */
uint8_t Clock_Init(uint32_t plln, uint32_t pllm, uint32_t pllp, uint32_t pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY)
    {
    }

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
    RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

    RCC_OscInitStruct.PLL.PLLN = plln;
    RCC_OscInitStruct.PLL.PLLM = pllm;
    RCC_OscInitStruct.PLL.PLLP = pllp;
    RCC_OscInitStruct.PLL.PLLQ = pllq;

    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if (ret != HAL_OK)
        return 1;

    // QSPI_Enable_Memmapmode(); //QSPI内存映射模式,需要在时钟初始化之前开启,否则会有各种问题

    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK |
                                   RCC_CLOCKTYPE_HCLK |
                                   RCC_CLOCKTYPE_D1PCLK1 |
                                   RCC_CLOCKTYPE_PCLK1 |
                                   RCC_CLOCKTYPE_PCLK2 |
                                   RCC_CLOCKTYPE_D3PCLK1);

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV4;
    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
    if (ret != HAL_OK)
        return 1;

    __HAL_RCC_CSI_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    HAL_EnableCompensationCell();

    return 0;
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

/**
  * @brief  QSPI 进入内存映射模式
  */
void QSPI_Enable_Memmapmode(void)
{
    uint32_t tempreg = 0;
    volatile uint32_t *data_reg = &QUADSPI->DR;
    GPIO_InitTypeDef qspi_gpio;

    RCC->AHB4ENR |= 1 << 1;  //使能 PORTB 时钟
    RCC->AHB4ENR |= 1 << 5;  //使能 PORTF 时钟
    RCC->AHB3ENR |= 1 << 14; //QSPI 时钟使能

    qspi_gpio.Pin = GPIO_PIN_6; //PB6 AF10
    qspi_gpio.Mode = GPIO_MODE_AF_PP;
    qspi_gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    qspi_gpio.Pull = GPIO_NOPULL;
    qspi_gpio.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOB, &qspi_gpio);

    qspi_gpio.Pin = GPIO_PIN_2; //PB2 AF9
    qspi_gpio.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &qspi_gpio);

    qspi_gpio.Pin = GPIO_PIN_6 | GPIO_PIN_7; //PF6 7 AF9
    qspi_gpio.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOF, &qspi_gpio);

    qspi_gpio.Pin = GPIO_PIN_8 | GPIO_PIN_9; //PF8 9 AF10
    qspi_gpio.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOF, &qspi_gpio);

    //QSPI 设置，参考 QSPI 实验的 QSPI_Init 函数
    RCC->AHB3RSTR |= 1 << 14;    //复位 QSPI
    RCC->AHB3RSTR &= ~(1 << 14); //停止复位 QSPI
    while (QUADSPI->SR & (1 << 5))
        ;                      //等待 BUSY 位清零
    QUADSPI->CR = 0X01000310;  //设置 CR 寄存器
    QUADSPI->DCR = 0X00160401; //设置 DCR 寄存器
    QUADSPI->CR |= 1 << 0;     //使能 QSPI

    //注意：QSPI QE 位的使能，在 QSPI 烧写算法里面，就已经设置了
    //所以，这里可以不用设置 QE 位，否则需要加入对 QE 位置 1 的代码
    //不过，代码必须通过仿真器下载，直接烧录到外部 QSPI FLASH，是不可用的
    //如果想直接烧录到外部 QSPI FLASH 也可以用，则需要在这里添加 QE 位置 1 的代码

    //W25QXX 进入 QPI 模式（0X38指令）
    while (QUADSPI->SR & (1 << 5))
        ;                      //等待 BUSY 位清零
    QUADSPI->CCR = 0X00000138; //发送 0X38 指令，W25QXX 进入 QPI 模式
    while ((QUADSPI->SR & (1 << 1)) == 0)
        ;                   //等待指令发送完成
    QUADSPI->FCR |= 1 << 1; //清除发送完成标志位

    //W25QXX 写使能（0X06指令）
    while (QUADSPI->SR & (1 << 5))
        ;                      //等待 BUSY 位清零
    QUADSPI->CCR = 0X00000106; //发送 0X06 指令，W25QXX 写使能
    while ((QUADSPI->SR & (1 << 1)) == 0)
        ;                   //等待指令发送完成
    QUADSPI->FCR |= 1 << 1; //清除发送完成标志位

    //W25QXX 设置 QPI 相关读参数（0XC0）
    while (QUADSPI->SR & (1 << 5))
        ;                      //等待 BUSY 位清零
    QUADSPI->CCR = 0X030003C0; //发送 0XC0 指令，W25QXX 读参数设置
    QUADSPI->DLR = 0;
    while ((QUADSPI->SR & (1 << 2)) == 0)
        ;                                   //等待 FTF
    *(volatile uint8_t *)data_reg = 3 << 4; //设置 P4&P5=11，8 个 dummy clocks，104M
    QUADSPI->CR |= 1 << 2;                  //终止传输
    while ((QUADSPI->SR & (1 << 1)) == 0)
        ;                   //等待数据发送完成
    QUADSPI->FCR |= 1 << 1; //清除发送完成标志位
    while (QUADSPI->SR & (1 << 5))
        ; //等待 BUSY 位清零

    //MemroyMap 模式设置
    while (QUADSPI->SR & (1 << 5))
        ;                   //等待 BUSY 位清零
    QUADSPI->ABR = 0;       //交替字节设置为 0，实际上就是 W25Q 0XEB 指令的 M0~M7=0
    tempreg = 0XEB;         //INSTRUCTION[7:0]=0XEB，发送 0XEB 指令（Fast Read QUAD I/O）
    tempreg |= 3 << 8;      //IMODE[1:0]=3，四线传输指令
    tempreg |= 3 << 10;     //ADDRESS[1:0]=3，四线传输地址
    tempreg |= 2 << 12;     //ADSIZE[1:0]=2，24位地址长度
    tempreg |= 3 << 14;     //ABMODE[1:0]=3，四线传输交替字节
    tempreg |= 0 << 16;     //ABSIZE[1:0]=0，8位交替字节 (M0~M7)
    tempreg |= 6 << 18;     //DCYC[4:0]=6，6个dummy周期
    tempreg |= 3 << 24;     //DMODE[1:0]=3，四线传输数据
    tempreg |= 3 << 26;     //FMODE[1:0]=3，内存映射模式
    QUADSPI->CCR = tempreg; //设置CCR寄存器

    //设置 QSPI FLASH 空间的MPU保护
    SCB->SHCSR &= ~(1 << 16);        //禁止 MemManage
    MPU->CTRL &= ~(1 << 0);          //禁止 MPU
    MPU->RNR = 0;                    //设置保护区域编号为 0 (1~7 可以给其他内存用)
    MPU->RBAR = 0X90000000;          //基地址为 0X9000 000，即 QSPI 的起始地址
    MPU->RASR = 0X0303002D;          //设置相关保护参数(禁止共用，允许 cache，允许缓冲)
    MPU->CTRL = (1 << 2) | (1 << 0); //使能 PRIVDEFENA，使能 MPU
    SCB->SHCSR |= 1 << 16;           //使能 MemManage
}

#if defined(__clang__) //使用V6编译器(clang)
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI
void __attribute__((noinline)) WFI_SET(void)
{
    __asm__("wfi");
}

//关闭所有中断(但是不包括fault和NMI中断)
void __attribute__((noinline)) INTX_DISABLE(void)
{
    __asm__("cpsid i \t\n"
            "bx lr");
}

//开启所有中断
void __attribute__((noinline)) INTX_ENABLE(void)
{
    __asm__("cpsie i \t\n"
            "bx lr");
}

//设置栈顶地址
//addr:栈顶地址
void __attribute__((noinline)) MSR_MSP(uint32_t addr)
{
    __asm__("msr msp, r0 \t\n"
            "bx r14");
}
#elif defined(__CC_ARM) //使用V5编译器(ARMCC)

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI
__asm void WFI_SET(void)
{
    WFI;
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
    CPSID I
        BX LR
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
    CPSIE I
        BX LR
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(uint32_t addr)
{
    MSR MSP, r0 //set Main Stack value
                 BX r14
}

#endif
