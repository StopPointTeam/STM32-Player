//UART 库

#include "stdio.h"

#include "sys.h"
#include "systick.h"

#include "uart.h"

COM_PORT_E printf_Com = COM1;
COM_PORT_E getchar_Com = COM1;

//USART1 PA9 PA10
#define USART1_CLK_ENABLE() __HAL_RCC_USART1_CLK_ENABLE()

#define USART1_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART1_TX_GPIO_PORT GPIOA
#define USART1_TX_PIN GPIO_PIN_9
#define USART1_TX_AF GPIO_AF7_USART1

#define USART1_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART1_RX_GPIO_PORT GPIOA
#define USART1_RX_PIN GPIO_PIN_10
#define USART1_RX_AF GPIO_AF7_USART1

//USART2 PD5 PD6
#define USART2_CLK_ENABLE() __HAL_RCC_USART2_CLK_ENABLE()

#define USART2_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define USART2_TX_GPIO_PORT GPIOD
#define USART2_TX_PIN GPIO_PIN_5
#define USART2_TX_AF GPIO_AF7_USART2

#define USART2_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define USART2_RX_GPIO_PORT GPIOD
#define USART2_RX_PIN GPIO_PIN_6
#define USART2_RX_AF GPIO_AF7_USART2

//USART3 PB10 PB11
#define USART3_CLK_ENABLE() __HAL_RCC_USART3_CLK_ENABLE()

#define USART3_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define USART3_TX_GPIO_PORT GPIOB
#define USART3_TX_PIN GPIO_PIN_10
#define USART3_TX_AF GPIO_AF7_USART3

#define USART3_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define USART3_RX_GPIO_PORT GPIOB
#define USART3_RX_PIN GPIO_PIN_11
#define USART3_RX_AF GPIO_AF7_USART3

//UART4 PC10 PC11
#define UART4_CLK_ENABLE() __HAL_RCC_UART4_CLK_ENABLE()

#define UART4_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART4_TX_GPIO_PORT GPIOC
#define UART4_TX_PIN GPIO_PIN_10
#define UART4_TX_AF GPIO_AF8_UART4

#define UART4_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART4_RX_GPIO_PORT GPIOC
#define UART4_RX_PIN GPIO_PIN_11
#define UART4_RX_AF GPIO_AF8_UART4

//UART5 PC12 PD2
#define UART5_CLK_ENABLE() __HAL_RCC_UART5_CLK_ENABLE()

#define UART5_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART5_TX_GPIO_PORT GPIOC
#define UART5_TX_PIN GPIO_PIN_12
#define UART5_TX_AF GPIO_AF8_UART5

#define UART5_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define UART5_RX_GPIO_PORT GPIOD
#define UART5_RX_PIN GPIO_PIN_2
#define UART5_RX_AF GPIO_AF8_UART5

//USART6 PG14 PC7
#define USART6_CLK_ENABLE() __HAL_RCC_USART6_CLK_ENABLE()

#define USART6_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
#define USART6_TX_GPIO_PORT GPIOG
#define USART6_TX_PIN GPIO_PIN_14
#define USART6_TX_AF GPIO_AF7_USART6

#define USART6_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define USART6_RX_GPIO_PORT GPIOC
#define USART6_RX_PIN GPIO_PIN_7
#define USART6_RX_AF GPIO_AF7_USART6

//UART7 PB4 PB3
#define UART7_CLK_ENABLE() __HAL_RCC_UART7_CLK_ENABLE()

#define UART7_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define UART7_TX_GPIO_PORT GPIOB
#define UART7_TX_PIN GPIO_PIN_4
#define UART7_TX_AF GPIO_AF11_UART7

#define UART7_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define UART7_RX_GPIO_PORT GPIOB
#define UART7_RX_PIN GPIO_PIN_3
#define UART7_RX_AF GPIO_AF11_UART7

//UART8 PJ8 PJ9
#define UART8_CLK_ENABLE() __HAL_RCC_UART8_CLK_ENABLE()

#define UART8_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOJ_CLK_ENABLE()
#define UART8_TX_GPIO_PORT GPIOJ
#define UART8_TX_PIN GPIO_PIN_8
#define UART8_TX_AF GPIO_AF8_UART8

#define UART8_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOJ_CLK_ENABLE()
#define UART8_RX_GPIO_PORT GPIOJ
#define UART8_RX_PIN GPIO_PIN_9
#define UART8_RX_AF GPIO_AF8_UART8

//定义每个串口结构体变量
#if UART1_FIFO_EN == 1
static UART_T g_tUart1;
static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE]; //发送缓冲区
static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE]; //接收缓冲区
#endif

#if UART2_FIFO_EN == 1
static UART_T g_tUart2;
static uint8_t g_TxBuf2[UART2_TX_BUF_SIZE]; //发送缓冲区
static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE]; //接收缓冲区
#endif

#if UART3_FIFO_EN == 1
static UART_T g_tUart3;
static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE]; //发送缓冲区
static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE]; //接收缓冲区
#endif

#if UART4_FIFO_EN == 1
static UART_T g_tUart4;
static uint8_t g_TxBuf4[UART4_TX_BUF_SIZE]; //发送缓冲区
static uint8_t g_RxBuf4[UART4_RX_BUF_SIZE]; //接收缓冲区
#endif

#if UART5_FIFO_EN == 1
static UART_T g_tUart5;
static uint8_t g_TxBuf5[UART5_TX_BUF_SIZE]; //发送缓冲区
static uint8_t g_RxBuf5[UART5_RX_BUF_SIZE]; //接收缓冲区
#endif

#if UART6_FIFO_EN == 1
static UART_T g_tUart6;
static uint8_t g_TxBuf6[UART6_TX_BUF_SIZE]; //发送缓冲区
static uint8_t g_RxBuf6[UART6_RX_BUF_SIZE]; //接收缓冲区
#endif

#if UART7_FIFO_EN == 1
static UART_T g_tUart7;
static uint8_t g_TxBuf7[UART7_TX_BUF_SIZE]; //发送缓冲区
static uint8_t g_RxBuf7[UART7_RX_BUF_SIZE]; //接收缓冲区
#endif

#if UART8_FIFO_EN == 1
static UART_T g_tUart8;
static uint8_t g_TxBuf8[UART8_TX_BUF_SIZE]; //发送缓冲区
static uint8_t g_RxBuf8[UART8_RX_BUF_SIZE]; //接收缓冲区
#endif

static void UartVarInit(void);
static void InitHardUart(void);
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);
static void UartIRQ(UART_T *_pUart);

/**
  * @brief  初始化串口
  */
void Uart_Init(void)
{
    UartVarInit(); //初始化全局变量

    InitHardUart(); //配置串口的硬件参数
}

/**
  * @brief  将 COM 端口号转换为 UART 指针
  * @param  _ucPort: 端口号 (COM1-8)
  * @retval UART 指针
  */
UART_T *ComToUart(COM_PORT_E _ucPort)
{
    if (_ucPort == COM1)
    {
#if UART1_FIFO_EN == 1
        return &g_tUart1;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM2)
    {
#if UART2_FIFO_EN == 1
        return &g_tUart2;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM3)
    {
#if UART3_FIFO_EN == 1
        return &g_tUart3;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM4)
    {
#if UART4_FIFO_EN == 1
        return &g_tUart4;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM5)
    {
#if UART5_FIFO_EN == 1
        return &g_tUart5;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM6)
    {
#if UART6_FIFO_EN == 1
        return &g_tUart6;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM7)
    {
#if UART7_FIFO_EN == 1
        return &g_tUart7;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM8)
    {
#if UART8_FIFO_EN == 1
        return &g_tUart8;
#else
        return 0;
#endif
    }
    else
    {
        return 0;
    }
}

/**
  * @brief  将 COM 端口号转换为 USART_TypeDef*
  * @param  _ucPort: 端口号 (COM1-8)
  * @retval USART_TypeDef*
  */
USART_TypeDef *ComToUSARTx(COM_PORT_E _ucPort)
{
    if (_ucPort == COM1)
    {
#if UART1_FIFO_EN == 1
        return USART1;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM2)
    {
#if UART2_FIFO_EN == 1
        return USART2;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM3)
    {
#if UART3_FIFO_EN == 1
        return USART3;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM4)
    {
#if UART4_FIFO_EN == 1
        return UART4;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM5)
    {
#if UART5_FIFO_EN == 1
        return UART5;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM6)
    {
#if UART6_FIFO_EN == 1
        return USART6;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM7)
    {
#if UART7_FIFO_EN == 1
        return UART7;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM8)
    {
#if UART8_FIFO_EN == 1
        return UART8;
#else
        return 0;
#endif
    }
    else
    {
        return 0;
    }
}

/**
  * @brief  向串口发送一组数据。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
  * @param  _ucPort: 端口号 (COM1-8)
  * @param  _ucaBuf: 待发送的数据缓冲区
  * @param  _usLen : 数据长度
  */
void Uart_SendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return;
    }

    if (pUart->SendBefore != 0)
    {
        pUart->SendBefore();
    }

    UartSend(pUart, _ucaBuf, _usLen);
}

/**
  * @brief  向串口发送 1 个字节。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
  * @param  _ucPort: 端口号 (COM1-8)
  * @param  _ucByte: 待发送的数据
  */
void Uart_SendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
    Uart_SendBuf(_ucPort, &_ucByte, 1);
}

/**
  * @brief  从接收缓冲区读取 1 字节，非阻塞。无论有无数据均立即返回
  * @param  _ucPort: 端口号 (COM1-8)
  * @param  _pByte: 接收到的数据指针
  * @retval 0 表示无数据，1 表示读取到有效字节
  */
uint8_t Uart_GetChar(COM_PORT_E _ucPort, uint8_t *_pByte)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return 0;
    }

    return UartGetChar(pUart, _pByte);
}

/**
  * @brief  从接收缓冲区读取到特定字节，阻塞，有超时
  * @param  _ucPort: 端口号 (COM1-8)
  * @param  _pBuf: 接收到的数据指针
  * @param  _endByte: 终止字节
  * @param  _timeout: 超时时间
  * @retval 0 表示超时，1 表示成功
  */
uint8_t Uart_GetBuffUntil(COM_PORT_E _ucPort, uint8_t *_pBuf, uint8_t _endByte, uint16_t _timeout)
{
    uint8_t ch;
    int32_t start_t = SysTick_GetRunTime();
    uint16_t i = 0;

    while (1)
    {
        if (Uart_GetChar(_ucPort, &ch) == 1)
        {
            if (ch == _endByte)
            {
                _pBuf[i] = '\0';
                return 1;
            }

            _pBuf[i++] = ch;
        }
        else if (SysTick_GetRunTime() - start_t > _timeout)
        {
            return 0;
        }
    }
}

/**
  * @brief  清零串口发送缓冲区
  * @param  _ucPort: 端口号 (COM1-8)
  */
void Uart_ClearTxFifo(COM_PORT_E _ucPort)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return;
    }

    pUart->usTxWrite = 0;
    pUart->usTxRead = 0;
    pUart->usTxCount = 0;
}

/**
  * @brief  清零串口接收缓冲区
  * @param  _ucPort: 端口号 (COM1-8)
  */
void Uart_ClearRxFifo(COM_PORT_E _ucPort)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return;
    }

    pUart->usRxWrite = 0;
    pUart->usRxRead = 0;
    pUart->usRxCount = 0;
}

/**
  * @brief  设置串口的波特率。无校验，收发都使能
  * @param  _ucPort: 端口号 (COM1-8)
  * @param  _BaudRate: 波特率
  */
void Uart_SetBaud(COM_PORT_E _ucPort, uint32_t _BaudRate)
{
    USART_TypeDef *USARTx;

    USARTx = ComToUSARTx(_ucPort);
    if (USARTx == 0)
    {
        return;
    }

    Uart_SetUartParam(USARTx, _BaudRate, UART_PARITY_NONE, UART_MODE_TX_RX);
}

/**
  * @brief  设置串口发送前处理函数
  * @param  _ucPort: 端口号 (COM1-8)
  * @param  Receive: 处理函数
  */
void Uart_BindSendBeforeHandle(COM_PORT_E _ucPort, UartSendBeforeHandler SendBefore)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return;
    }

    pUart->SendBefore = SendBefore;
}

/**
  * @brief  设置串口发送后处理函数
  * @param  _ucPort: 端口号 (COM1-8)
  * @param  Receive: 处理函数
  */
void Uart_BindSendOverHandle(COM_PORT_E _ucPort, UartSendOverHandler SendOver)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return;
    }

    pUart->SendOver = SendOver;
}

/**
  * @brief  设置串口接受中断处理函数
  * @param  _ucPort: 端口号 (COM1-8)
  * @param  Receive: 中断处理函数
  */
void Uart_BindReceiveHandle(COM_PORT_E _ucPort, UartReceiveHandler Receive)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return;
    }

    pUart->Receive = Receive;
}

/**
  * @brief  初始化串口相关的变量
  */
static void UartVarInit(void)
{
#if UART1_FIFO_EN == 1
    g_tUart1.uart = USART1;                   //串口设备
    g_tUart1.pTxBuf = g_TxBuf1;               //发送缓冲区指针
    g_tUart1.pRxBuf = g_RxBuf1;               //接收缓冲区指针
    g_tUart1.usTxBufSize = UART1_TX_BUF_SIZE; //发送缓冲区大小
    g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE; //接收缓冲区大小
    g_tUart1.usTxWrite = 0;                   //发送FIFO写索引
    g_tUart1.usTxRead = 0;                    //发送FIFO读索引
    g_tUart1.usRxWrite = 0;                   //接收FIFO写索引
    g_tUart1.usRxRead = 0;                    //接收FIFO读索引
    g_tUart1.usRxCount = 0;                   //接收到的新数据个数
    g_tUart1.usTxCount = 0;                   //待发送的数据个数
    g_tUart1.SendBefore = 0;                  //发送数据前的回调函数
    g_tUart1.SendOver = 0;                    //发送完毕后的回调函数
    g_tUart1.Receive = 0;                     //接收到新数据后的回调函数
    g_tUart1.Sending = 0;                     //正在发送中标志
#endif

#if UART2_FIFO_EN == 1
    g_tUart2.uart = USART2;                   //串口设备
    g_tUart2.pTxBuf = g_TxBuf2;               //发送缓冲区指针
    g_tUart2.pRxBuf = g_RxBuf2;               //接收缓冲区指针
    g_tUart2.usTxBufSize = UART2_TX_BUF_SIZE; //发送缓冲区大小
    g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE; //接收缓冲区大小
    g_tUart2.usTxWrite = 0;                   //发送FIFO写索引
    g_tUart2.usTxRead = 0;                    //发送FIFO读索引
    g_tUart2.usRxWrite = 0;                   //接收FIFO写索引
    g_tUart2.usRxRead = 0;                    //接收FIFO读索引
    g_tUart2.usRxCount = 0;                   //接收到的新数据个数
    g_tUart2.usTxCount = 0;                   //待发送的数据个数
    g_tUart2.SendBefore = 0;                  //发送数据前的回调函数
    g_tUart2.SendOver = 0;                    //发送完毕后的回调函数
    g_tUart2.Receive = 0;                     //接收到新数据后的回调函数
    g_tUart2.Sending = 0;                     //正在发送中标志
#endif

#if UART3_FIFO_EN == 1
    g_tUart3.uart = USART3;
    g_tUart3.pTxBuf = g_TxBuf3;
    g_tUart3.pRxBuf = g_RxBuf3;
    g_tUart3.usTxBufSize = UART3_TX_BUF_SIZE;
    g_tUart3.usRxBufSize = UART3_RX_BUF_SIZE;
    g_tUart3.usTxWrite = 0;
    g_tUart3.usTxRead = 0;
    g_tUart3.usRxWrite = 0;
    g_tUart3.usRxRead = 0;
    g_tUart3.usRxCount = 0;
    g_tUart3.usTxCount = 0;
    g_tUart3.SendBefore = 0;
    g_tUart3.SendOver = 0;
    g_tUart3.Receive = 0;
    g_tUart3.Sending = 0;
#endif

#if UART4_FIFO_EN == 1
    g_tUart4.uart = UART4;
    g_tUart4.pTxBuf = g_TxBuf4;
    g_tUart4.pRxBuf = g_RxBuf4;
    g_tUart4.usTxBufSize = UART4_TX_BUF_SIZE;
    g_tUart4.usRxBufSize = UART4_RX_BUF_SIZE;
    g_tUart4.usTxWrite = 0;
    g_tUart4.usTxRead = 0;
    g_tUart4.usRxWrite = 0;
    g_tUart4.usRxRead = 0;
    g_tUart4.usRxCount = 0;
    g_tUart4.usTxCount = 0;
    g_tUart4.SendBefore = 0;
    g_tUart4.SendOver = 0;
    g_tUart4.Receive = 0;
    g_tUart4.Sending = 0;
#endif

#if UART5_FIFO_EN == 1
    g_tUart5.uart = UART5;
    g_tUart5.pTxBuf = g_TxBuf5;
    g_tUart5.pRxBuf = g_RxBuf5;
    g_tUart5.usTxBufSize = UART5_TX_BUF_SIZE;
    g_tUart5.usRxBufSize = UART5_RX_BUF_SIZE;
    g_tUart5.usTxWrite = 0;
    g_tUart5.usTxRead = 0;
    g_tUart5.usRxWrite = 0;
    g_tUart5.usRxRead = 0;
    g_tUart5.usRxCount = 0;
    g_tUart5.usTxCount = 0;
    g_tUart5.SendBefore = 0;
    g_tUart5.SendOver = 0;
    g_tUart5.Receive = 0;
    g_tUart5.Sending = 0;
#endif

#if UART6_FIFO_EN == 1
    g_tUart6.uart = USART6;
    g_tUart6.pTxBuf = g_TxBuf6;
    g_tUart6.pRxBuf = g_RxBuf6;
    g_tUart6.usTxBufSize = UART6_TX_BUF_SIZE;
    g_tUart6.usRxBufSize = UART6_RX_BUF_SIZE;
    g_tUart6.usTxWrite = 0;
    g_tUart6.usTxRead = 0;
    g_tUart6.usRxWrite = 0;
    g_tUart6.usRxRead = 0;
    g_tUart6.usRxCount = 0;
    g_tUart6.usTxCount = 0;
    g_tUart6.SendBefore = 0;
    g_tUart6.SendOver = 0;
    g_tUart6.Receive = 0;
    g_tUart6.Sending = 0;
#endif

#if UART7_FIFO_EN == 1
    g_tUart7.uart = UART7;
    g_tUart7.pTxBuf = g_TxBuf7;
    g_tUart7.pRxBuf = g_RxBuf7;
    g_tUart7.usTxBufSize = UART7_TX_BUF_SIZE;
    g_tUart7.usRxBufSize = UART7_RX_BUF_SIZE;
    g_tUart7.usTxWrite = 0;
    g_tUart7.usTxRead = 0;
    g_tUart7.usRxWrite = 0;
    g_tUart7.usRxRead = 0;
    g_tUart7.usRxCount = 0;
    g_tUart7.usTxCount = 0;
    g_tUart7.SendBefore = 0;
    g_tUart7.SendOver = 0;
    g_tUart7.Receive = 0;
    g_tUart7.Sending = 0;
#endif

#if UART8_FIFO_EN == 1
    g_tUart8.uart = UART8;
    g_tUart8.pTxBuf = g_TxBuf8;
    g_tUart8.pRxBuf = g_RxBuf8;
    g_tUart8.usTxBufSize = UART8_TX_BUF_SIZE;
    g_tUart8.usRxBufSize = UART8_RX_BUF_SIZE;
    g_tUart8.usTxWrite = 0;
    g_tUart8.usTxRead = 0;
    g_tUart8.usRxWrite = 0;
    g_tUart8.usRxRead = 0;
    g_tUart8.usRxCount = 0;
    g_tUart8.usTxCount = 0;
    g_tUart8.SendBefore = 0;
    g_tUart8.SendOver = 0;
    g_tUart8.Receive = 0;
    g_tUart8.Sending = 0;
#endif
}

/**
  * @brief  配置串口的硬件参数
  * @param  Instance: USART_TypeDef*
  * @param  BaudRate: 波特率
  * @param  Parity: 校验类型，奇校验或者偶校验
  * @param  Mode: 发送和接收模式使能
  */
void Uart_SetUartParam(USART_TypeDef *Instance, uint32_t BaudRate, uint32_t Parity, uint32_t Mode)
{
    UART_HandleTypeDef UartHandle;

    UartHandle.Instance = Instance;

    UartHandle.Init.BaudRate = BaudRate;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    UartHandle.Init.Parity = Parity;
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode = Mode;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    UartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    UartHandle.Init.ClockPrescaler = UART_PRESCALER_DIV1;

    UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    HAL_UART_Init(&UartHandle);
}

/**
  * @brief  配置串口的硬件参数
  */
static void InitHardUart(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

    /* 
       默认情况下，USART1 和 USART6 选择的 PCLK2，时钟 100MHz
       USART2 USART3 UART4 UART5 UART6 UART7 UART8 选择的时钟是 PLCK1，时钟 100MHz
    */
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART16;
    RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

#if UART1_FIFO_EN == 1
    USART1_TX_GPIO_CLK_ENABLE();
    USART1_RX_GPIO_CLK_ENABLE();

    USART1_CLK_ENABLE();

    GPIO_InitStruct.Pin = USART1_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART1_TX_AF;
    HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USART1_RX_PIN;
    GPIO_InitStruct.Alternate = USART1_RX_AF;
    HAL_GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStruct);

    //配置NVIC the NVIC for UART
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    //配置波特率、奇偶校验
    Uart_SetUartParam(USART1, UART1_BAUD, UART_PARITY_NONE, UART_MODE_TX_RX);

    SET_BIT(USART1->ICR, USART_ICR_TCCF);   //清除发送完成标志
    SET_BIT(USART1->RQR, USART_RQR_RXFRQ);  //清除接收标志
    SET_BIT(USART1->CR1, USART_CR1_RXNEIE); //使能接受中断
#endif

#if UART2_FIFO_EN == 1
    USART2_TX_GPIO_CLK_ENABLE();
    USART2_RX_GPIO_CLK_ENABLE();

    USART2_CLK_ENABLE();

    GPIO_InitStruct.Pin = USART2_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART2_TX_AF;
    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USART2_RX_PIN;
    GPIO_InitStruct.Alternate = USART2_RX_AF;
    HAL_GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USART2_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    Uart_SetUartParam(USART2, UART2_BAUD, UART_PARITY_NONE, UART_MODE_TX_RX);

    SET_BIT(USART2->ICR, USART_ICR_TCCF);
    SET_BIT(USART2->RQR, USART_RQR_RXFRQ);
    SET_BIT(USART2->CR1, USART_CR1_RXNEIE);
#endif

#if UART3_FIFO_EN == 1
    USART3_TX_GPIO_CLK_ENABLE();
    USART3_RX_GPIO_CLK_ENABLE();

    USART3_CLK_ENABLE();

    GPIO_InitStruct.Pin = USART3_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART3_TX_AF;
    HAL_GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USART3_RX_PIN;
    GPIO_InitStruct.Alternate = USART3_RX_AF;
    HAL_GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USART3_IRQn, 0, 3);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    Uart_SetUartParam(USART3, UART3_BAUD, UART_PARITY_NONE, UART_MODE_TX_RX);

    SET_BIT(USART3->ICR, USART_ICR_TCCF);
    SET_BIT(USART3->RQR, USART_RQR_RXFRQ);
    SET_BIT(USART3->CR1, USART_CR1_RXNEIE);
#endif

#if UART4_FIFO_EN == 1
    UART4_TX_GPIO_CLK_ENABLE();
    UART4_RX_GPIO_CLK_ENABLE();

    UART4_CLK_ENABLE();

    GPIO_InitStruct.Pin = UART4_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = UART4_TX_AF;
    HAL_GPIO_Init(UART4_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = UART4_RX_PIN;
    GPIO_InitStruct.Alternate = UART4_RX_AF;
    HAL_GPIO_Init(UART4_RX_GPIO_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(UART4_IRQn, 0, 4);
    HAL_NVIC_EnableIRQ(UART4_IRQn);

    Uart_SetUartParam(UART4, UART4_BAUD, UART_PARITY_NONE, UART_MODE_TX_RX);

    SET_BIT(UART4->ICR, USART_ICR_TCCF);
    SET_BIT(UART4->RQR, USART_RQR_RXFRQ);
    SET_BIT(UART4->CR1, USART_CR1_RXNEIE);
#endif

#if UART5_FIFO_EN == 1
    UART5_TX_GPIO_CLK_ENABLE();
    UART5_RX_GPIO_CLK_ENABLE();

    UART5_CLK_ENABLE();

    GPIO_InitStruct.Pin = UART5_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = UART5_TX_AF;
    HAL_GPIO_Init(UART5_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = UART5_RX_PIN;
    GPIO_InitStruct.Alternate = UART5_RX_AF;
    HAL_GPIO_Init(UART5_RX_GPIO_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(UART5_IRQn, 0, 5);
    HAL_NVIC_EnableIRQ(UART5_IRQn);

    Uart_SetUartParam(UART5, UART5_BAUD, UART_PARITY_NONE, UART_MODE_TX_RX);

    SET_BIT(UART5->ICR, USART_ICR_TCCF);
    SET_BIT(UART5->RQR, USART_RQR_RXFRQ);
    SET_BIT(UART5->CR1, USART_CR1_RXNEIE);
#endif

#if UART6_FIFO_EN == 1
    USART6_TX_GPIO_CLK_ENABLE();
    USART6_RX_GPIO_CLK_ENABLE();

    USART6_CLK_ENABLE();

    GPIO_InitStruct.Pin = USART6_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART6_TX_AF;
    HAL_GPIO_Init(USART6_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USART6_RX_PIN;
    GPIO_InitStruct.Alternate = USART6_RX_AF;
    HAL_GPIO_Init(USART6_RX_GPIO_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USART6_IRQn, 0, 6);
    HAL_NVIC_EnableIRQ(USART6_IRQn);

    Uart_SetUartParam(USART6, UART6_BAUD, UART_PARITY_NONE, UART_MODE_TX_RX);

    SET_BIT(USART6->ICR, USART_ICR_TCCF);
    SET_BIT(USART6->RQR, USART_RQR_RXFRQ);
    SET_BIT(USART6->CR1, USART_CR1_RXNEIE);
#endif

#if UART7_FIFO_EN == 1
    UART7_TX_GPIO_CLK_ENABLE();
    UART7_RX_GPIO_CLK_ENABLE();

    UART7_CLK_ENABLE();

    GPIO_InitStruct.Pin = UART7_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = UART7_TX_AF;
    HAL_GPIO_Init(UART7_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = UART7_RX_PIN;
    GPIO_InitStruct.Alternate = UART7_RX_AF;
    HAL_GPIO_Init(UART7_RX_GPIO_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(UART7_IRQn, 0, 6);
    HAL_NVIC_EnableIRQ(UART7_IRQn);

    Uart_SetUartParam(UART7, UART7_BAUD, UART_PARITY_NONE, UART_MODE_TX_RX);

    SET_BIT(UART7->ICR, USART_ICR_TCCF);
    SET_BIT(UART7->RQR, USART_RQR_RXFRQ);
    SET_BIT(UART7->CR1, USART_CR1_RXNEIE);
#endif

#if UART8_FIFO_EN == 1
    UART8_TX_GPIO_CLK_ENABLE();
    UART7_RX_GPIO_CLK_ENABLE();

    UART8_CLK_ENABLE();

    GPIO_InitStruct.Pin = UART8_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = UART8_TX_AF;
    HAL_GPIO_Init(UART8_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = UART8_RX_PIN;
    GPIO_InitStruct.Alternate = UART8_RX_AF;
    HAL_GPIO_Init(UART8_RX_GPIO_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(UART8_IRQn, 0, 6);
    HAL_NVIC_EnableIRQ(UART8_IRQn);

    Uart_SetUartParam(UART8, UART8_BAUD, UART_PARITY_NONE, UART_MODE_TX_RX);

    SET_BIT(UART8->ICR, USART_ICR_TCCF);
    SET_BIT(UART8->RQR, USART_RQR_RXFRQ);
    SET_BIT(UART8->CR1, USART_CR1_RXNEIE);
#endif
}

/**
  * @brief  填写数据到 UART 发送缓冲区，并启动发送中断。中断处理函数发送完毕后，自动关闭发送中断
  * @param  _pUart
  * @param  _ucaBuf
  * @param  _usLen
  */
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
    uint16_t i;

    for (i = 0; i < _usLen; i++)
    {
        //如果发送缓冲区已满，则等待缓冲区空
        while (1)
        {
            __IO uint16_t usCount;

            DISABLE_INT();
            usCount = _pUart->usTxCount;
            ENABLE_INT();

            if (usCount < _pUart->usTxBufSize)
            {
                break;
            }
            else if (usCount == _pUart->usTxBufSize) //数据已填满缓冲区
            {
                if ((_pUart->uart->CR1 & USART_CR1_TXEIE) == 0)
                {
                    SET_BIT(_pUart->uart->CR1, USART_CR1_TXEIE);
                }
            }
        }

        //将新数据填入发送缓冲区
        _pUart->pTxBuf[_pUart->usTxWrite] = _ucaBuf[i];

        DISABLE_INT();
        if (++_pUart->usTxWrite >= _pUart->usTxBufSize)
        {
            _pUart->usTxWrite = 0;
        }
        _pUart->usTxCount++;
        ENABLE_INT();
    }

    SET_BIT(_pUart->uart->CR1, USART_CR1_TXEIE); //使能发送中断（缓冲区空）
}

/**
  * @brief  填写数据到 UART 发送缓冲区，并启动发送中断。中断处理函数发送完毕后，自动关闭发送中断
  * @param  _pUart
  * @param  _pByte
  * @retval 成功返回 1，失败返回 0
  */
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
    uint16_t usCount;

    DISABLE_INT();
    usCount = _pUart->usRxCount;
    ENABLE_INT();

    //如果读和写索引相同，则返回 0
    if (usCount == 0)
    {
        return 0;
    }
    else
    {
        *_pByte = _pUart->pRxBuf[_pUart->usRxRead]; //从串口接收 FIFO 取 1 个数据

        //改写 FIFO 读索引
        DISABLE_INT();
        if (++_pUart->usRxRead >= _pUart->usRxBufSize)
        {
            _pUart->usRxRead = 0;
        }
        _pUart->usRxCount--;
        ENABLE_INT();
        return 1;
    }
}

/**
  * @brief  判断发送缓冲区是否为空
  * @param  _ucPort
  * @retval 1 为空，0 为不空
  */
uint8_t Uart_IsTxEmpty(COM_PORT_E _ucPort)
{
    UART_T *pUart;
    uint8_t Sending;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return 0;
    }

    Sending = pUart->Sending;

    if (Sending != 0)
    {
        return 0;
    }
    return 1;
}

/**
  * @brief  供中断服务程序调用，通用串口中断处理函数
  * @param  _pUart
  */
static void UartIRQ(UART_T *_pUart)
{
    uint32_t isrflags = READ_REG(_pUart->uart->ISR);
    uint32_t cr1its = READ_REG(_pUart->uart->CR1);
    uint32_t cr3its = READ_REG(_pUart->uart->CR3);

    //处理接收中断
    if ((isrflags & UART_FLAG_RXNE) != RESET)
    {
        //从串口接收数据寄存器读取数据存放到接收 FIFO
        uint8_t ch = READ_REG(_pUart->uart->RDR);
        _pUart->pRxBuf[_pUart->usRxWrite] = ch;
        if (++_pUart->usRxWrite >= _pUart->usRxBufSize)
            _pUart->usRxWrite = 0;
        if (_pUart->usRxCount < _pUart->usRxBufSize)
            _pUart->usRxCount++;

        //回调函数
        if (_pUart->Receive)
            _pUart->Receive(ch);
    }

    //处理发送缓冲区空中断
    if (((isrflags & UART_FLAG_TXE) != RESET) && (cr1its & USART_CR1_TXEIE) != RESET)
    {
        if (_pUart->usTxCount == 0)
        {
            //发送缓冲区的数据已取完时，禁止发送缓冲区空中断
            CLEAR_BIT(_pUart->uart->CR1, USART_CR1_TXEIE);

            //使能数据发送完毕中断
            SET_BIT(_pUart->uart->CR1, USART_CR1_TCIE);
        }
        else
        {
            _pUart->Sending = 1;

            //从发送 FIFO 取 1 个字节写入串口发送数据寄存器
            _pUart->uart->TDR = _pUart->pTxBuf[_pUart->usTxRead];
            if (++_pUart->usTxRead >= _pUart->usTxBufSize)
            {
                _pUart->usTxRead = 0;
            }
            _pUart->usTxCount--;
        }
    }

    //数据 bit 位全部发送完毕的中断
    if (((isrflags & USART_ISR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
    {
        if (_pUart->usTxCount == 0)
        {
            //如果发送 FIFO 的数据全部发送完毕，禁止数据发送完毕中断
            CLEAR_BIT(_pUart->uart->CR1, USART_CR1_TCIE);

            //回调函数
            if (_pUart->SendOver)
            {
                _pUart->SendOver();
            }

            _pUart->Sending = 0;
        }
        else
        {
            //如果发送 FIFO 的数据还未完毕，则从发送 FIFO 取 1 个数据写入发送数据寄存器
            _pUart->uart->TDR = _pUart->pTxBuf[_pUart->usTxRead];
            if (++_pUart->usTxRead >= _pUart->usTxBufSize)
            {
                _pUart->usTxRead = 0;
            }
            _pUart->usTxCount--;
        }
    }

    //清除中断标志
    SET_BIT(_pUart->uart->ICR, UART_CLEAR_PEF);
    SET_BIT(_pUart->uart->ICR, UART_CLEAR_FEF);
    SET_BIT(_pUart->uart->ICR, UART_CLEAR_NEF);
    SET_BIT(_pUart->uart->ICR, UART_CLEAR_OREF);
    SET_BIT(_pUart->uart->ICR, UART_CLEAR_IDLEF);
    SET_BIT(_pUart->uart->ICR, UART_CLEAR_TCF);
    SET_BIT(_pUart->uart->ICR, UART_CLEAR_LBDF);
    SET_BIT(_pUart->uart->ICR, UART_CLEAR_CTSF);
    SET_BIT(_pUart->uart->ICR, UART_CLEAR_CMF);
    SET_BIT(_pUart->uart->ICR, UART_CLEAR_WUF);
    SET_BIT(_pUart->uart->ICR, UART_CLEAR_TXFECF);
}

#if UART1_FIFO_EN == 1
void USART1_IRQHandler(void)
{
    UartIRQ(&g_tUart1);
}
#endif

#if UART2_FIFO_EN == 1
void USART2_IRQHandler(void)
{
    UartIRQ(&g_tUart2);
}
#endif

#if UART3_FIFO_EN == 1
void USART3_IRQHandler(void)
{
    UartIRQ(&g_tUart3);
}
#endif

#if UART4_FIFO_EN == 1
void UART4_IRQHandler(void)
{
    UartIRQ(&g_tUart4);
}
#endif

#if UART5_FIFO_EN == 1
void UART5_IRQHandler(void)
{
    UartIRQ(&g_tUart5);
}
#endif

#if UART6_FIFO_EN == 1
void USART6_IRQHandler(void)
{
    UartIRQ(&g_tUart6);
}
#endif

#if UART7_FIFO_EN == 1
void UART7_IRQHandler(void)
{
    UartIRQ(&g_tUart7);
}
#endif

#if UART8_FIFO_EN == 1
void UART8_IRQHandler(void)
{
    UartIRQ(&g_tUart8);
}
#endif

void Uart_SetprintfCom(COM_PORT_E _ucPort)
{
    printf_Com = _ucPort;
}

void Uart_SetgetcharCom(COM_PORT_E _ucPort)
{
    getchar_Com = _ucPort;
}

//重定义 fputc 函数，用于 printf
int fputc(int ch, FILE *f)
{
    Uart_SendChar(printf_Com, ch);

    return ch;
}

//重定义 fgetc 函数，用于 getchar
int fgetc(FILE *f)
{
    uint8_t ucData;

    while (Uart_GetChar(getchar_Com, &ucData) == 0)
        ;

    return ucData;
}
