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
		void startSending();
		void stopListening();
		~uartConnection();
	private:
		int fd;
		static bool isListening;
		termios configSetup(int fd);
};
