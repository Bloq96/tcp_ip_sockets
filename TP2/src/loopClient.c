#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"
#include "structures.h"

#define SERVER_PORT 54321
#define ATTEMPTS 100000

int main(int argc, char *argv[]) {
    char *hostIP;

    if(argc>1) {
        hostIP = argv[1];
        if(validIPv4(hostIP,stringLength(hostIP))!=1) {
            printf("Invalid IP format.\n");
            return 1;
        }
    } else {
        printf("Using default IP (127.0.0.1).\n");
        hostIP = "127.0.0.1";
    }

    int socketId;
    
    socketId = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketId < 0) {
        printf("Could not open socket.\n");
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
    zeroFill((char *)&serverAddress.sin_zero,
    sizeof(serverAddress.sin_zero));

    FILE *outputFile = fopen("output.csv","w");
    struct Buffer buffer;
    charFill(buffer.data, BUFFER_SIZE, 'Z');
    unsigned int serverAddressLength = sizeof(serverAddress);
    int values[43] = {1, 100, 200, 300, 400, 500, 600, 700, 800, 900,
    1000, 1024, 2048, 3072, 4096, 5120, 6144, 7168, 8192, 9216,
    10240, 11264, 12288, 13312, 14336, 15360, 16384, 17408, 18432,
    19456, 20480, 21504, 22528, 23552, 24576, 25600, 26624, 27648,
    28672, 29696, 30720, 31744, 32768};
    int times;
    struct timespec startTime, endTime;
    long long unsigned int latency[43];
    zeroFill((char *)latency, sizeof(latency));

    fprintf(outputFile,"Data (bytes),Latency (ns),Troughput (bps)\n");
    for(int it=0;it<43;++it) {
        buffer.data[values[it]-1] = '\0';
        for(times = 0; times<ATTEMPTS; ++times) {
            clock_gettime(CLOCK_MONOTONIC_RAW, &startTime);
            sendto(socketId, buffer.data, values[it], 0,
            (struct sockaddr *)&serverAddress, sizeof(serverAddress));
            buffer.length = recvfrom(socketId, buffer.data,
            sizeof(buffer.data), 0, (struct sockaddr *)&serverAddress,
            &serverAddressLength);
            clock_gettime(CLOCK_MONOTONIC_RAW, &endTime);
            latency[it] += 1000000000*(endTime.tv_sec-
            startTime.tv_sec)+(endTime.tv_nsec-startTime.tv_nsec);
        }
        latency[it] /= ATTEMPTS;
        buffer.data[buffer.length-1] = 'Z';
        fprintf(outputFile,"%d,%lld,%g\n", values[it], latency[it],
        (8.0*((double)values[it]))/
        (((double)latency[it])/1000000000.0));
    }
    fclose(outputFile);
    close(socketId);
}
