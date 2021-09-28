//UART ��

#ifndef __UART_H
#define __UART_H

#define UART1_FIFO_EN 1 //�����ô���
#define UART2_FIFO_EN 1 //WiFi ͸��
#define UART3_FIFO_EN 0
#define UART4_FIFO_EN 0
#define UART5_FIFO_EN 0
#define UART6_FIFO_EN 0
#define UART7_FIFO_EN 0
#define UART8_FIFO_EN 0

//����˿ں�
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

//���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫��
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

//�ص���������
typedef void (*UartSendBeforeHandler)(void);
typedef void (*UartSendOverHandler)(void);
typedef void (*UartReceiveHandler)(uint8_t _byte);

//�����豸�ṹ��
typedef struct
{
    USART_TypeDef *uart;     //�ڲ������豸ָ��
    uint8_t *pTxBuf;         //���ͻ�����
    uint8_t *pRxBuf;         //���ջ�����
    uint16_t usTxBufSize;    //���ͻ�������С
    uint16_t usRxBufSize;    //���ջ�������С
    __IO uint16_t usTxWrite; //���ͻ�����дָ��
    __IO uint16_t usTxRead;  //���ͻ�������ָ��
    __IO uint16_t usTxCount; //�ȴ����͵����ݸ���

    __IO uint16_t usRxWrite; //���ջ�����дָ��
    __IO uint16_t usRxRead;  //���ջ�������ָ��
    __IO uint16_t usRxCount; //��δ��ȡ�������ݸ���

    UartSendBeforeHandler SendBefore; //��ʼ����֮ǰ�Ļص�����ָ��
    UartSendOverHandler SendOver;     //������ϵĻص�����ָ��
    UartReceiveHandler Receive;       //�����յ����ݵĻص�����ָ��

    uint8_t Sending; //���ڷ�����
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
