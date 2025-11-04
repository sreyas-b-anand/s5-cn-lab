#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define LOCALHOST "127.0.0.1"
#define TOTAL_FRAMES 10

typedef struct {
    int fNo;
    int data;
} Packet;

typedef struct {
    int aNo;
} Ack;

int main() {
    Packet p;
    Ack a;
    int buffer[TOTAL_FRAMES + 1] = {0};
    int dropped_once[TOTAL_FRAMES + 1] = {0};
    int num_received = 0;

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) { perror("Socket"); exit(1); }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(LOCALHOST);
    socklen_t s_size = sizeof(server_address);

    if (bind(s, (struct sockaddr *)&server_address, s_size) < 0) {
        perror("Bind");
        exit(1);
    }

    struct sockaddr_in client_address;
    socklen_t c_size = sizeof(client_address);

    printf("Receiver ready...\n");

    while (1) {
        int rcv = recvfrom(s, &p, sizeof(Packet), 0, (struct sockaddr *)&client_address, &c_size);
        if (rcv < 0) continue;

        // Drop every 3rd frame only once
        if (p.fNo % 3 == 0 && dropped_once[p.fNo] == 0) {
            dropped_once[p.fNo] = 1;
            printf("Dropped frame: %d (first time)\n", p.fNo);
            continue;  // Do not ACK this time
        }

        // Process and ACK normally
        if (buffer[p.fNo] == 0) {
            buffer[p.fNo] = p.data;
            num_received++;
            printf("Frame received: %d\n", p.fNo);
        }

        a.aNo = p.fNo;
        sendto(s, &a, sizeof(Ack), 0, (struct sockaddr *)&client_address, c_size);
        printf("ACK sent for frame %d\n", p.fNo);

        if (num_received == TOTAL_FRAMES) break;
    }
    for(int i = 1 ; i <= TOTAL_FRAMES ; i++ ){
        printf("---------------------------Data in the buffer \n--------------------------------------\n");
        printf("[%d , %d]" , i , buffer[i]);
    }

    close(s);
    return 0;
}
