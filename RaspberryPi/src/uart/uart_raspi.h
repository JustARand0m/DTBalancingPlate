#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstdio>
#include <poll.h>
#include <csignal>
#include <string.h>

#define BUFFERSIZE 1024

class uartConnection {
	public:
		uartConnection();
		void startListening();
		void writeData(std::string input);
		void stopListening();
		~uartConnection();

	private:
		int fd;
		termios configSetup(int fd);
		bool isListening;
};
