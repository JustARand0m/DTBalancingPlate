#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "socketReceiver.h"

void socketReceiver::startListening() {
    while (true) {
        bufferLock.lock();
        // read content into buffer from an incoming client
        int len = recvfrom(sock, buffer, sizeof(buffer), 0,
                           (struct sockaddr *)&client_address,
			(socklen_t*)&client_address_len);
        bufferLock.unlock();
	usleep(1);
    }
}

std::string socketReceiver::getData() {

    bufferLock.lock();
    
    std::string tempBuffer(buffer, 50); 
    bufferLock.unlock();

    return tempBuffer;
}
 
socketReceiver::socketReceiver() {
    // port to start the server on
    int SERVER_PORT = 1337;
 
    // socket address used for the server
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
 
    // htons: host to network short: transforms a value in host byte
    // ordering format to a short value in network byte ordering format
    server_address.sin_port = htons(SERVER_PORT);
 
    // htons: host to network long: same as htons but to long
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
 
    // create a UDP socket, creation returns -1 on failure
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("could not create socket\n");
	exit(1);
    }
 
    // bind it to listen to the incoming connections on the created server
    // address, will return -1 on error
    if ((bind(sock, (struct sockaddr *)&server_address,
              sizeof(server_address))) < 0) {
        printf("could not bind socket\n");
        exit(1);
    }
 
    int client_address_len = 0;
}

socketReceiver* startSocketReceiver() {
	socketReceiver *rev = new socketReceiver;
  
   std::thread t(&socketReceiver::startListening, rev); 
   t.detach(); 
	return rev;
}
