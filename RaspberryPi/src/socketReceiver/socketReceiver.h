#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <mutex>
#include <thread>
#include <iostream>



class socketReceiver {
    public:
        socketReceiver();
        void startListening();

	std::string getData();
    private:
        int sock;
        struct sockaddr_in client_address;
        int client_address_len;
        std::mutex bufferLock;
        char buffer[500] ={'\0'};
};


socketReceiver *startSocketReceiver();
