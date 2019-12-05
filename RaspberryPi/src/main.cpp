#include <iostream>
#include "uart/uart_raspi.h"
#include "../include/exceptionHandling.h"

int main() {
	uartConnection uartCon("/dev/serial0");
	uartCon.startListening();
	while(true) {
		sleep(10);
		uartCon.writeData("12345678");
		std::cout << "data written" << std::endl;
	}

	return 0;
}
