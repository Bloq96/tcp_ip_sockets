#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "functions.h"
#include "structures.h"

#define SERVER_PORT 54321
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    char *hostIP;

    if(argc>1) {
        hostIP = argv[1];
        if(validIPv4(hostIP,stringLength(hostIP))!=1) {
            printf("connectTo: Invalid IP format.\n");
            return 1;
        }
    } else {
        printf("connectTo: Using default IP (127.0.0.1).\n");
        hostIP = "127.0.0.1";
    }

    struct sockaddr_in serverAddress;

    // IPv4
    serverAddress.sin_family = AF_INET;
    // IPv4 address
    inet_pton(AF_INET, hostIP,
    (struct in_addr *)&serverAddress.sin_addr);
    // TCP port
    serverAddress.sin_port = htons(SERVER_PORT);
    // Zeros
    bzero(&serverAddress.sin_zero,sizeof(serverAddress.sin_zero));

    int socketId;
    
    printf("connectTo: Opening socket.\n");
    socketId = socket(AF_INET, SOCK_STREAM, 0);
    if(socketId < 0) {
        printf("connectTo: Could not open socket.\n");
        return 1;
    }

    printf("connectTo: Trying to establish connection.\n");
    if(connect(socketId, (struct sockaddr *)&serverAddress,
    sizeof(serverAddress))) {
        printf("connectTo: Server unreachable.");
        close(socketId);
        return 1;
    }
    
    struct Buffer buffer;

    while(1) {
        printf("connectTo: Type message.\n");
        buffer.length = readLine(buffer.data,BUFFER_SIZE);
        while(buffer.length<0) {
            buffer.length = readLine(buffer.data,BUFFER_SIZE);
        }
        if(buffer.length&&compareStrings((char *)buffer.data, "END",
        buffer.length)==0) {
            break;
        }
        printf("connectTo: Sending data.\n");
        send(socketId, buffer.data, buffer.length+1, 0);
    }
    shutdown(socketId, SHUT_RDWR);
    close(socketId);
}
