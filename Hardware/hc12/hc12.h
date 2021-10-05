//HC12 ¿â

#ifndef __HC12_H
#define __HC12_H

#include "uart.h"

#define HC12_COM COM3

#define HC12_SendBuff(__pdata__, __data_len__) Uart_SendBuf(HC12_COM, __pdata__, __data_len__)
#define HC12_Receive(__pdata__) Uart_GetChar(HC12_COM, __pdata__)
#define HC12_ReceiveBuffUntil(__pdata__, __end_byte__, __timeout__) Uart_GetBuffUntil(HC12_COM, __pdata__, __end_byte__, __timeout__)
#define HC12_ClearSend Uart_ClearTxFifo(HC12_COM)
#define HC12_ClearReceive Uart_ClearRxFifo(HC12_COM)
#define HC12_BindReceiveHandle(__receive__) Uart_BindReceiveHandle(HC12_COM, __receive__)

void HC12_Init(void);

#endif
