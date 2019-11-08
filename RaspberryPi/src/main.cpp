#include <iostream>
#include "uart/uart_raspi.h"
#include "../include/exceptionHandling.h"

int main() {
	uartConnection uartCon("/dev/ttyS0");
	uartCon.writeData("test");

	return 0;
}
