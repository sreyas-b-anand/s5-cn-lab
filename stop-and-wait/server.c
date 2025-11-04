#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER 256
#define LOCALHOST "127.0.0.1"
#define TOTAL_FRAMES 10


typedef struct {
    int frameNo;
    int data;
} Packet;

typedef struct {
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
    int seq = 1, flag = 1;
    Packet p;
    Ack a;
    char recvBuffer[BUFFER];

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    errorcheck(s, "socket");

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(LOCALHOST);
    socklen_t s_size = sizeof(server_address);

    int b = bind(s , (struct sockaddr *)&server_address , s_size);
    errorcheck(b , "bind done");

    struct sockaddr_in client_address;
    socklen_t c_size = sizeof(client_address);

    while(seq <= TOTAL_FRAMES){
        int rcv = recvfrom(s , &p , sizeof(Packet) , 0 , (struct sockaddr *)&client_address , &c_size);
        if(rcv > 0){
            printf("Frame received : %d\n" , p.frameNo );
            if(p.frameNo % 3 == 0 && flag == 1){
                printf("Frame %d dropped \n" , p.frameNo);
                printf("Server in sleep mode\n");
                sleep(7);
                flag = 0;
                continue;
            }else{
                if(p.frameNo == seq){
                    a.ackNo = seq;
                    sendto(s , &a , sizeof(Ack) , 0 , (struct sockaddr *)&client_address , c_size);
                    printf("ACK %d sent\n" , seq);
                    seq++;
                    flag = 1;
                    continue;
                }else{
                    a.ackNo = seq-1;
                    seq++;
                    flag = 1;
                    sendto(s , &a , sizeof(Ack) , 0 , (struct sockaddr *)&client_address , c_size);
                    printf("ACK %d sent\n" , seq);
                }
            }
        }else{
            printf("Error recieving frames\n");
        }
    }

    return 0;
}