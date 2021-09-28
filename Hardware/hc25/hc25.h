//HC25 ¿â

#ifndef __HC25_H
#define __HC25_H

#include "sys.h"
#include "uart.h"

#define HC25_COM COM2

#define HC25_SendBuff(__pdata__, __data_len__)       \
    Uart_SendBuf(HC25_COM, __pdata__, __data_len__); \
    Delay_ms(50)
#define HC25_Receive(__pdata__) Uart_GetChar(HC25_COM, __pdata__)
#define HC25_ReceiveBuffUntil(__pdata__, __end_byte__, __timeout__) Uart_GetBuffUntil(HC25_COM, __pdata__, __end_byte__, __timeout__)
#define HC25_ClearSend Uart_ClearTxFifo(HC25_COM)
#define HC25_ClearReceive Uart_ClearRxFifo(HC25_COM)
#define HC25_EnterATMode         \
    if (is_at_mode == 0)         \
    {                            \
        HC25_SendBuff("+++", 4); \
        Delay_ms(200);            \
        is_at_mode = 1;          \
        HC25_ClearReceive;       \
    }
#define HC25_ExitATMode         \
    if (is_at_mode == 1)        \
    {                           \
        HC25_SendATCmd("ENTM"); \
        Delay_ms(200);           \
        is_at_mode = 0;         \
        HC25_ClearReceive;      \
    }

void HC25_Init(void);

void HC25_SendATCmd(uint8_t *cmd);

#endif
