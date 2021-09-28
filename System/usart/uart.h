//UART 库

#ifndef __UART_H
#define __UART_H

#define UART1_FIFO_EN 1 //调试用串口
#define UART2_FIFO_EN 1 //WiFi 透传
#define UART3_FIFO_EN 0
#define UART4_FIFO_EN 0
#define UART5_FIFO_EN 0
#define UART6_FIFO_EN 0
#define UART7_FIFO_EN 0
#define UART8_FIFO_EN 0

//定义端口号
typedef enum
{
    COM1 = 0, //USART1
    COM2 = 1, //USART2
    COM3 = 2, //USART3
    COM4 = 3, //UART4
    COM5 = 4, //UART5
    COM6 = 5, //USART6
    COM7 = 6, //UART7
    COM8 = 7  //UART8
} COM_PORT_E;

//定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工
#if UART1_FIFO_EN == 1
#define UART1_BAUD 115200
#define UART1_TX_BUF_SIZE 1 * 1024
#define UART1_RX_BUF_SIZE 1 * 1024
#endif

#if UART2_FIFO_EN == 1
#define UART2_BAUD 115200
#define UART2_TX_BUF_SIZE 1 * 1024
#define UART2_RX_BUF_SIZE 1 * 1024
#endif

#if UART3_FIFO_EN == 1
#define UART3_BAUD 115200
#define UART3_TX_BUF_SIZE 1 * 1024
#define UART3_RX_BUF_SIZE 1 * 1024
#endif

#if UART4_FIFO_EN == 1
#define UART4_BAUD 115200
#define UART4_TX_BUF_SIZE 1 * 1024
#define UART4_RX_BUF_SIZE 1 * 1024
#endif

#if UART5_FIFO_EN == 1
#define UART5_BAUD 115200
#define UART5_TX_BUF_SIZE 1 * 1024
#define UART5_RX_BUF_SIZE 1 * 1024
#endif

#if UART6_FIFO_EN == 1
#define UART6_BAUD 115200
#define UART6_TX_BUF_SIZE 1 * 1024
#define UART6_RX_BUF_SIZE 1 * 1024
#endif

#if UART7_FIFO_EN == 1
#define UART7_BAUD 115200
#define UART7_TX_BUF_SIZE 1 * 1024
#define UART7_RX_BUF_SIZE 1 * 1024
#endif

#if UART8_FIFO_EN == 1
#define UART8_BAUD 115200
#define UART8_TX_BUF_SIZE 1 * 1024
#define UART8_RX_BUF_SIZE 1 * 1024
#endif

//回调函数定义
typedef void (*UartSendBeforeHandler)(void);
typedef void (*UartSendOverHandler)(void);
typedef void (*UartReceiveHandler)(uint8_t _byte);

//串口设备结构体
typedef struct
{
    USART_TypeDef *uart;     //内部串口设备指针
    uint8_t *pTxBuf;         //发送缓冲区
    uint8_t *pRxBuf;         //接收缓冲区
    uint16_t usTxBufSize;    //发送缓冲区大小
    uint16_t usRxBufSize;    //接收缓冲区大小
    __IO uint16_t usTxWrite; //发送缓冲区写指针
    __IO uint16_t usTxRead;  //发送缓冲区读指针
    __IO uint16_t usTxCount; //等待发送的数据个数

    __IO uint16_t usRxWrite; //接收缓冲区写指针
    __IO uint16_t usRxRead;  //接收缓冲区读指针
    __IO uint16_t usRxCount; //还未读取的新数据个数

    UartSendBeforeHandler SendBefore; //开始发送之前的回调函数指针
    UartSendOverHandler SendOver;     //发送完毕的回调函数指针
    UartReceiveHandler Receive;       //串口收到数据的回调函数指针

    uint8_t Sending; //正在发送中
} UART_T;

void Uart_Init(void);
void Uart_SendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void Uart_SendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t Uart_GetChar(COM_PORT_E _ucPort, uint8_t *_pByte);
uint8_t Uart_GetBuffUntil(COM_PORT_E _ucPort, uint8_t *_pBuf, uint8_t _endByte, uint16_t _timeout);
void Uart_ClearTxFifo(COM_PORT_E _ucPort);
void Uart_ClearRxFifo(COM_PORT_E _ucPort);
void Uart_SetBaud(COM_PORT_E _ucPort, uint32_t _BaudRate);
void Uart_SetUartParam(USART_TypeDef *Instance, uint32_t BaudRate, uint32_t Parity, uint32_t Mode);
uint8_t Uart_IsTxEmpty(COM_PORT_E _ucPort);

void Uart_BindSendBeforeHandle(COM_PORT_E _ucPort, UartSendBeforeHandler SendBefore);
void Uart_BindSendOverHandle(COM_PORT_E _ucPort, UartSendOverHandler SendOver);
void Uart_BindReceiveHandle(COM_PORT_E _ucPort, UartReceiveHandler Receive);

void Uart_SetprintfCom(COM_PORT_E _ucPort);
void Uart_SetgetcharCom(COM_PORT_E _ucPort);

#endif
