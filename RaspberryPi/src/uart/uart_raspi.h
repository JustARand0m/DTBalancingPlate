#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstdio>
#include <poll.h>
#include <csignal>
#include <cstring>
#include <thread>
#include <mutex>
#include <vector>
#include <cstdint>
#include "../messageDecoding/messageDecoding.h"

#define MESSAGE_ID_PLATE 0
#define MESSAGE_ID_SERVO 1

class uartConnection {
	public:
		uartConnection(std::string device);
		void startListening();
		static void listeningThread(int &fd, bool &isLitening, const int BUFFSZ, 
				std::mutex &lock, std::mutex &dataAccess, messageDecoding &decode, std::vector<int> &data);
		void writeData(std::string input);
		void stopListening();
		~uartConnection();
		std::vector<int> getData();

	private:
		termios configSetup(int fd);

		int fd;
		const int BUFFERSIZE;
		bool isListening;
		std::vector<int> data;
		std::mutex lock;
		std::mutex dataAccess;
		messageDecoding decode;
};
