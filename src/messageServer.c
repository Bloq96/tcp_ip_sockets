#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "structures.h"

#define SERVER_PORT 54321
#define BUFFER_SIZE 256

int breakProcess = 0;

void signalHandler(int s) {
    printf("Caught signal %d\n",s);
    breakProcess = 1;
}

int main(int argc, char *argv[]) {
    printf("messageServer: Creating socket.\n");
    int socketId = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(socketId < 0) {
        printf("messageServer: Could not create socket.\n");
        return 1;
    }

    struct sockaddr_in serverAddress;

    // IPv4
    serverAddress.sin_family = AF_INET;
    // IPv4 address
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    // TCP port
    serverAddress.sin_port = htons(SERVER_PORT);
    // Zeros
    bzero(&serverAddress.sin_zero,sizeof(serverAddress.sin_zero));

    printf("messageServer: Binding socket to current machine.\n");
    if(bind(socketId, (struct sockaddr *)&serverAddress,
    sizeof(serverAddress))<0) {
        printf("messageServer: Could not enable socket.\n");
        close(socketId);
        return 1;
    }

    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = signalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    char addressString[16];
    struct sockaddr_in clientAddress;
    struct Buffer buffer;
    unsigned int clientAddressLength = sizeof(clientAddress);

    while(!breakProcess) {
        printf("messageServer: Receiving message.\n");
        buffer.length = recvfrom(socketId, buffer.data,
        sizeof(buffer.data), 0, (struct sockaddr *)&clientAddress,
        &clientAddressLength);
        if(breakProcess) break;
        while(buffer.length) {
            inet_ntop(AF_INET,
            (struct in_addr *)&clientAddress.sin_addr,
            addressString, sizeof(addressString));
            printf("messageServer: Client (%s:%d) says: %s\n",
            addressString, ntohs(clientAddress.sin_port),
            buffer.data);
            printf("messageServer: Replying message.\n");
            sendto(socketId, buffer.data, buffer.length, 0,
            (struct sockaddr *)&clientAddress, sizeof(clientAddress));
            printf("messageServer: Receiving message.\n");
            if(breakProcess) break;
            buffer.length = recvfrom(socketId, buffer.data,
            sizeof(buffer.data), 0, (struct sockaddr *)&clientAddress,
            &clientAddressLength);
            if(breakProcess) break;
        }
    }
    close(socketId);
}
