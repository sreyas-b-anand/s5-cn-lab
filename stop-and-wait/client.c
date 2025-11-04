#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8080
#define BUFFER 256
#define LOCALHOST "127.0.0.1"
#define TOTAL_FRAMES 10

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

    int seq = 1, sent = 0;
    Packet p;
    Ack a;
    char recvFrom[BUFFER];

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    errorcheck(s, "socket done");

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(LOCALHOST);
    socklen_t s_size = sizeof(server_address);

    struct sockaddr_in client_address;
    socklen_t c_size = sizeof(client_address);

    struct timeval timeout = {5, 0};
    errorcheck(setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)), "timeout done");
    
    while (seq <= TOTAL_FRAMES)
    {
        
        p.frameNo = seq; 
        p.data = rand() % 100;
        sent = 0;
        while (!sent)
        {
            int snd = sendto(s, &p, sizeof(Packet), 0, (struct sockaddr *)&server_address, s_size);
            if (snd > 0)
            {
                printf("Sent frame %d\n" , p.frameNo);
            }
            int rcv = recvfrom(s, &a, sizeof(Ack), 0, (struct sockaddr *)&client_address, &c_size);
            if (rcv > 0)
            {
                printf("ACK received for frame : %d\n" , a.ackNo);
                sent = 1;
                seq++;
            }
            else
            {
                printf("Timeout... resending!!!\n");
            }
        }
    }

    return 0;
}