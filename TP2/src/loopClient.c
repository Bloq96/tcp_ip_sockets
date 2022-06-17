#include <arpa/inet.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"
#include "structures.h"

#define ATTEMPTS 100

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

    struct pollfd pollId;

    pollId.fd = socketId;
    pollId.events = POLLIN;
    
    FILE *outputFile = fopen("output.csv","w");
    struct Buffer input, output;
    charFill(output.data, BUFFER_SIZE, 'Z');
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
    int errors = 0;
    int progress = 0;
    char bar[21];
    progressBar(progress, bar, sizeof(bar));

    fprintf(outputFile,"Data (bytes),Latency (ns),Troughput (bps)\n");
    for(int it=0;it<43;++it) {
        output.length = values[it]-1;
        output.data[output.length] = '\0';
        errors = 0;
        for(times = 0; times<ATTEMPTS; ++times) {
            clock_gettime(CLOCK_MONOTONIC_RAW, &startTime);
            sendto(socketId, output.data, values[it], 0,
            (struct sockaddr *)&serverAddress, sizeof(serverAddress));
            if(poll(&pollId,1,1000)<0) {
                printf("Poll error.\n");
                fclose(outputFile);
                close(socketId);
                return 1;
            }
            if((pollId.revents&POLLIN)==POLLIN) { 
                input.length = recvfrom(socketId, input.data,
                sizeof(input.data), 0,
                (struct sockaddr *)&serverAddress,
                &serverAddressLength);
            } else {
                --times;
                ++errors;
                printf("\r%d: [%s] (%d%%) E: %d", it, bar, progress,
                errors);
                fflush(stdout);
                continue;
            }
            clock_gettime(CLOCK_MONOTONIC_RAW, &endTime);
            if(input.length!=values[it]) {
                --times;
                ++errors;
                printf("\r%d: [%s] (%d%%) E: %d", it, bar, progress,
                errors);
                fflush(stdout);
                continue;
            }
            progress = ((times+1)*100)/ATTEMPTS;
            progressBar(progress, bar, sizeof(bar));
            printf("\r%d: [%s] (%d%%) E: %d", it, bar, progress,
            errors);
            fflush(stdout);
            latency[it] += 1000000000*(endTime.tv_sec-
            startTime.tv_sec)+(endTime.tv_nsec-startTime.tv_nsec);
        }
        printf("\n");
        latency[it] /= times;
        output.data[output.length] = 'Z';
        fprintf(outputFile,"%d,%lld,%g\n", values[it], latency[it],
        (8.0*((double)values[it]))/
        (((double)latency[it])/1000000000.0));
    }
    fclose(outputFile);
    close(socketId);
}
