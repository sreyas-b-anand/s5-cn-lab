#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define LOCALHOST "127.0.0.1"
#define BUFFER 256

void error_check(int val, char buffer[])
{
    if (val < 0)
    {
        printf("Error \n");
        exit(1);
    }
    else
        printf("%s\n", buffer);
}

int main()
{

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    error_check(s, "socket done");

    struct sockaddr_in client_address;
    client_address.sin_port = htons(PORT);
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(LOCALHOST);
    socklen_t client_size = sizeof(client_address);

    int snd = sendto(s, "fileContent", 11, 0, (struct sockaddr *)&client_address, client_size);
    if (snd > 0)
    {
        printf("Request sent\n");
    }
    else
    {
        printf("Error sending request\n");
    }

    char recv_buffer[BUFFER];

    int rcv = recvfrom(s, recv_buffer, BUFFER, 0, (struct sockaddr *)&client_address, &client_size);
    if (rcv > 0)
    {
        printf("Response received : %s\n", recv_buffer);
    }
    else
    {
        printf("Error receiving response\n");
    }
}