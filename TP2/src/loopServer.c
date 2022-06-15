#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "functions.h"
#include "structures.h"

#define SERVER_PORT 54321

int breakProcess = 0;

void updateValues(int *values, int length) {
    switch(length) {
        case 1:
            ++values[0];
            break;
        case 100:
            ++values[1];
            break;
        case 200:
            ++values[2];
            break;
        case 300:
            ++values[3];
            break;
        case 400:
            ++values[4];
            break;
        case 500:
            ++values[5];
            break;
        case 600:
            ++values[6];
            break;
        case 700:
            ++values[7];
            break;
        case 800:
            ++values[8];
            break;
        case 900:
            ++values[9];
            break;
        case 1000:
            ++values[10];
            break;
        case 1024:
            ++values[11];
            break;
        case 2048:
            ++values[12];
            break;
        case 3072:
            ++values[13];
            break;
        case 4096:
            ++values[14];
            break;
        case 5120:
            ++values[15];
            break;
        case 6144:
            ++values[16];
            break;
        case 7168:
            ++values[17];
            break;
        case 8192:
            ++values[18];
            break;
        case 9216:
            ++values[19];
            break;
        case 10240:
            ++values[20];
            break;
        case 11264:
            ++values[21];
            break;
        case 12288:
            ++values[22];
            break;
        case 13312:
            ++values[23];
            break;
        case 14336:
            ++values[24];
            break;
        case 15360:
            ++values[25];
            break;
        case 16384:
            ++values[26];
            break;
        case 17408:
            ++values[27];
            break;
        case 18432:
            ++values[28];
            break;
        case 19456:
            ++values[29];
            break;
        case 20480:
            ++values[30];
            break;
        case 21504:
            ++values[31];
            break;
        case 22528:
            ++values[32];
            break;
        case 23552:
            ++values[33];
            break;
        case 24576:
            ++values[34];
            break;
        case 25600:
            ++values[35];
            break;
        case 26624:
            ++values[36];
            break;
        case 27648:
            ++values[37];
            break;
        case 28672:
            ++values[38];
            break;
        case 29696:
            ++values[39];
            break;
        case 30720:
            ++values[40];
            break;
        case 31744:
            ++values[41];
            break;
        case 32768:
            ++values[42];
            break;
        default:
            ++values[43];
    }

}

void signalHandler(int s) {
    printf("Caught signal %d\n",s);
    breakProcess = 1;
}

int main(int argc, char *argv[]) {
    int socketId = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(socketId < 0) {
        printf("Could not create socket.\n");
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
    zeroFill((char *)&serverAddress.sin_zero,
    sizeof(serverAddress.sin_zero));

    if(bind(socketId, (struct sockaddr *)&serverAddress,
    sizeof(serverAddress))<0) {
        printf("Could not enable socket.\n");
        close(socketId);
        return 1;
    }

    struct timeval timeout;

    timeout.tv_sec = 5;  /* 5 Secs Timeout */

    if(setsockopt(socketId, SOL_SOCKET, SO_RCVTIMEO,
    (struct timeval *)&timeout, sizeof(struct timeval))<0) {
        printf("Could not set receive timeout option.\n");
        close(socketId);
        return 1;
    }

    if(setsockopt(socketId, SOL_SOCKET, SO_SNDTIMEO,
    (struct timeval *)&timeout, sizeof(struct timeval))<0) {
        printf("Could not set send timeout option.\n");
        close(socketId);
        return 1;
    }

    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = signalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    struct sockaddr_in clientAddress;
    struct Buffer buffer;
    unsigned int clientAddressLength = sizeof(clientAddress);
    int lengths[44];
    zeroFill((char *)lengths, sizeof(lengths));
    int errors = 0;

    while(!breakProcess) {
        buffer.length = recvfrom(socketId, buffer.data,
        sizeof(buffer.data), 0, (struct sockaddr *)&clientAddress,
        &clientAddressLength);
        if(breakProcess) break;
        if(buffer.length<0) {
            ++errors;
            continue;
        }
        while(buffer.length) {
            updateValues(lengths,stringLength(buffer.data)+1);
            if(sendto(socketId, buffer.data, buffer.length, 0,
            (struct sockaddr *)&clientAddress, sizeof(clientAddress))<0) {
                ++errors;
                continue;
            }
            if(breakProcess) break;
            buffer.length = recvfrom(socketId, buffer.data,
            sizeof(buffer.data), 0, (struct sockaddr *)&clientAddress,
            &clientAddressLength);
            if(breakProcess) break;
            if(buffer.length<0) {
                ++errors;
                continue;
            }
        }
    }
    int values[43] = {1, 100, 200, 300, 400, 500, 600, 700, 800, 900,
    1000, 1024, 2048, 3072, 4096, 5120, 6144, 7168, 8192, 9216,
    10240, 11264, 12288, 13312, 14336, 15360, 16384, 17408, 18432,
    19456, 20480, 21504, 22528, 23552, 24576, 25600, 26624, 27648,
    28672, 29696, 30720, 31744, 32768};

    for(int it=0;it<43;++it) {
        printf("%d: %d\n", values[it], lengths[it]);
    }
    printf("Errors: %d\n", lengths[43]+errors);

    close(socketId);
}
