#include <iostream>
#include "uart/uart_raspi.h"
#include "../include/exceptionHandling.h"

int main() {
	uartConection uartCon("/dev/serial0");
	uartCon.startListening();
	while(true) {
		uartCon.writeData("test");
	}

	return 0;
}
