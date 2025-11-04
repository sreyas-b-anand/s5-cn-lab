#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER 256
#define TOTAL_FRAMES 7
#define WINDOW_SIZE 3

typedef struct
{
    int frameNo;
    int data;
} Packet;

typedef struct
{
    int ackNo;
} Ack;

void errorcheck(int result, const char *message)
{
    if (result < 0)
    {
        perror(message);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("%s\n", message);
    }
}

int main()
{
    int base = 1, next = 1;

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    errorcheck(s, "socket done");

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t s_size = sizeof(server_address);

    struct sockaddr_in client_address;
    socklen_t c_size = sizeof(client_address);

    struct timeval t = {5, 0};
    errorcheck(setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t)), "tt");

    while (base <= TOTAL_FRAMES)
    {

        int expected_seq = 1;
        Ack a;

        while (next < base + WINDOW_SIZE && next <= TOTAL_FRAMES)
        {
            Packet p;
            p.frameNo = next;
            p.data = rand() % 100;
            int snd = sendto(s, &p, sizeof(Packet), 0, (struct sockaddr *)&server_address, s_size);
            printf("Frame sent : %d\n", p.frameNo);
            next++;
        }

        int rcv = recvfrom(s, &a, sizeof(Ack), 0, (struct sockaddr *)&client_address, &c_size);
        if (rcv > 0)
        {
            printf("ACK received : %d\n", a.ackNo);
            base++;
        }
        else
        {

            printf("Timeout error... resending frame : %d\n", base);
            next = base;
        }
    }

    close(s);
    return 0;
}
