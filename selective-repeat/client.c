#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define LOCALHOST "127.0.0.1"
#define TOTAL_FRAMES 10
#define WIN_SIZE 3

typedef struct
{
    int fNo;
    int data;
} Packet;

typedef struct
{
    int aNo;
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
    Packet p;
    Ack a;
    int acked[TOTAL_FRAMES + 1] = {0};
    int base = 1, next = 1;

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    errorcheck(s, "sockdone");

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(LOCALHOST);
    socklen_t s_size = sizeof server_address;

    struct sockaddr_in client_address;
    socklen_t c_size = sizeof client_address;

    struct timeval timeout = {3, 0};
    errorcheck(setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout), "tv setup");

    while (base <= TOTAL_FRAMES)
    {
        while (next < base + WIN_SIZE && next <= TOTAL_FRAMES)
        {
            p.fNo = next;
            p.data = rand() % 100 + 1;
            int snd = sendto(s, &p, sizeof(Packet), 0, (struct sockaddr *)&server_address, s_size);
            printf("Frame sent : %d\n", p.fNo);
            next++;
        }

        int rcv = recvfrom(s, &a, sizeof(Ack), 0, (struct sockaddr *)&client_address, &c_size);
        if (rcv > 0)
        {
            printf("Ack received : %d\n", a.aNo);
            acked[a.aNo] = 1;
            while (acked[base] && base <= TOTAL_FRAMES)
                base++;
        }
        else
        {
            printf("--------------------Timeout error... Resending frame %d\n", base);
            for (int i = base; i < next; i++)
            {
                if (acked[i] == 0)
                {
                    p.fNo = i;
                    p.data = (rand() % 100 + 1);
                    int snd = sendto(s, &p, sizeof(Packet), 0, (struct sockaddr *)&server_address, s_size);
                    printf("Frame sent : %d\n", p.fNo);
                }
            }
        }
    }

    return 0;
}