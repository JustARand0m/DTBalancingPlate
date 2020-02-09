#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstdio>
#include <poll.h>
#include <csignal>
#include <string.h>
#include <thread>
#include <mutex>


class uartConnection {
	public:
		uartConnection(std::string device);
		void startListening();
		static void listeningThread(int &fd, bool &isLitening, const int BUFFSZ, std::mutex &lock);
		void writeData(std::string input);
		void stopListening();
		~uartConnection();

	private:
		int fd;
		termios configSetup(int fd);
		bool isListening;
		const int BUFFERSIZE;
		std::mutex lock;
};
