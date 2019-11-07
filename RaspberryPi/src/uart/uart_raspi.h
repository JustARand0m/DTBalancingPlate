#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstdio>

class uartConnection {
	public:
		uartConnection();
		void connect();
		~uartConnection();
	private:
		int fd;
		termios configSetup(int fd);
};
