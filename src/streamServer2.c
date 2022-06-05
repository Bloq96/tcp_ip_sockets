#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "structures.h"

#define SERVER_PORT 54321
#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 256

int breakProcess = 0;

void signalHandler(int s) {
    printf("Caught signal %d\n",s);
    breakProcess = 1;
}

int main(int argc, char *argv[]) {
    struct sockaddr_in serverAddress;

    // IPv4
    serverAddress.sin_family = AF_INET;
    // IPv4 address
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    // TCP port
    serverAddress.sin_port = htons(SERVER_PORT);
    // Zeros
    bzero(&serverAddress.sin_zero,sizeof(serverAddress.sin_zero));

    printf("streamServer: Creating socket.\n");
    int listeningSocketId = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listeningSocketId < 0) {
        printf("streamServer: Could not create socket.\n");
        return 1;
    }

    printf("streamServer: Binding socket to current machine.\n");
    if(bind(listeningSocketId, (struct sockaddr *)&serverAddress,
    sizeof(serverAddress))<0) {
        printf("streamServer: Could not enable socket.\n");
        close(listeningSocketId);
        return 1;
    }

    printf("streamServer: Start listening.\n");
    listen(listeningSocketId, MAX_CONNECTIONS);

    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = signalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    int connectionSocketId;
    char addressString[16];
    struct sockaddr_in clientAddress;
    struct Buffer buffer;
    unsigned int clientAddressLength = sizeof(clientAddress);

    while(!breakProcess) {
        printf("streamServer: Accepting connections.\n");
        connectionSocketId = accept(listeningSocketId,
        (struct sockaddr *)&clientAddress, &clientAddressLength);
        if(connectionSocketId<0) {
            printf("streamServer: Failed to connect.\n");
            close(listeningSocketId);
            return 1;
        }
        if(breakProcess) break;
        inet_ntop(AF_INET,
        (struct in_addr *)&clientAddress.sin_addr,
        addressString, sizeof(addressString));
        
        printf("streamServer: Receiving message.\n");
        buffer.length = recv(connectionSocketId, buffer.data,
        sizeof(buffer.data), 0);
        if(breakProcess) break;
        while(buffer.length) {
            printf("streamServer: Client (%s:%d) says: %s\n",
            addressString, ntohs(clientAddress.sin_port),
            buffer.data);
            printf("streamServer: Replying message.\n");
            send(connectionSocketId, buffer.data, buffer.length, 0);
            if(breakProcess) break;
            printf("streamServer: Receiving message.\n");
            buffer.length = recv(connectionSocketId, buffer.data,
            sizeof(buffer.data), 0);
            if(breakProcess) break;
        }
        shutdown(connectionSocketId, SHUT_RDWR);
        close(connectionSocketId);
    }
    close(listeningSocketId);
}
