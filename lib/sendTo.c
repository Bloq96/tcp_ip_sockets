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
            printf("sendTo: Invalid IP format.\n");
            return 1;
        }
    } else {
        printf("sendTo: Using default IP (127.0.0.1).\n");
        hostIP = "127.0.0.1";
    }

    int socketId;
    
    printf("sendTo: Opening socket.\n");
    socketId = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketId < 0) {
        printf("sendTo: Could not open socket.\n");
        return 1;
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

    struct Buffer buffer;
    char addressString[16];
    unsigned int serverAddressLength = sizeof(serverAddress);

    while(1) {
        printf("sendTo: Type message.\n");
        buffer.length = readLine(buffer.data,BUFFER_SIZE);
        if(compareStrings((char *)buffer.data, "END", buffer.length)
        ==0) {
            break;
        }
        printf("sendTo: Sending data.\n");
        sendto(socketId, buffer.data, buffer.length+1, 0,
        (struct sockaddr *)&serverAddress, sizeof(serverAddress));
        printf("sendTo: Receiving data.\n");
        buffer.length = recvfrom(socketId, buffer.data,
        sizeof(buffer.data), 0, (struct sockaddr *)&serverAddress,
        &serverAddressLength);
        inet_ntop(AF_INET,
        (struct in_addr *)&serverAddress.sin_addr,
        addressString, sizeof(addressString));
        printf("sendTo: Server (%s:%d) says: %s\n", addressString,
        ntohs(serverAddress.sin_port), buffer.data);
    }
    close(socketId);
}
