#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "structures.h"

#define SERVER_PORT 54321
#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    struct sockaddr_in serverAddress;

    // IPv4
    serverAddress.sin_family = AF_INET;
    // IPv4 address
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    // TCP port
    serverAddress.sin_port = htons(SERVER_PORT);

    printf("messageServer: Creating socket.\n");
    int listeningSocketId = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listeningSocketId < 0) {
        printf("messageServer: Could not create socket.\n");
        return 1;
    }

    printf("messageServer: Binding socket to current machine.\n");
    if(bind(listeningSocketId, (struct sockaddr *)&serverAddress,
    sizeof(serverAddress))<0) {
        printf("messageServer: Could not enable socket.\n");
        close(listeningSocketId);
        return 1;
    }

    printf("messageServer: Start listening.\n");
    listen(listeningSocketId, MAX_CONNECTIONS);

    int connectionSocketId;
    struct sockaddr_in clientAddress;
    struct Buffer buffer;
    unsigned int clientAddressLength = sizeof(clientAddress);

    //while(1) {
        printf("messageServer: Accepting connections.\n");
        connectionSocketId = accept(listeningSocketId,
        (struct sockaddr *)&clientAddress, &clientAddressLength);
        if(connectionSocketId<0) {
            printf("messageServer: Failed to connect.\n");
            close(listeningSocketId);
            return 1;
        }
        
        printf("messageServer: Receiving message.\n");
        buffer.length = recv(connectionSocketId, buffer.data,
        sizeof(buffer.data), 0);
        while(buffer.length) {
            printf("%s\n", buffer.data); 
            buffer.length = recv(connectionSocketId, buffer.data,
            sizeof(buffer.data), 0);
        }
        shutdown(connectionSocketId, SHUT_RDWR);
        close(connectionSocketId);
    //}
    close(listeningSocketId);
}
