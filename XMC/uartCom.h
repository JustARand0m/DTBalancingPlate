
#ifndef UARTCOM_H_
#define UARTCOM_H_

#include "globals.h"
#include <stdlib.h>

void decodeData();
void irUartDataReceived();

void uart_noise();
void uart_stop0();
void uart_stop1();
void uart_sync();

#endif /* UARTCOM_H_ */
