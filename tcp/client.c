#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define LOCALHOST "127.0.0.1"
#define PORT 8080
#define BUFFER 256

void error_check(int val, char message[])
{
    if (val < 0)
    {
        perror(message);
        exit(1);
    }
    else
        printf("%s\n", message);
}

int main()
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    error_check(s, "socket done");

    struct sockaddr_in client_address;
    
	client_address.sin_family = AF_INET;
	client_address.sin_addr.s_addr = inet_addr(LOCALHOST);
	client_address.sin_port = htons(PORT);
    
    socklen_t client_len = sizeof(client_address);

    int c = connect(s, (struct sockaddr *)&client_address, client_len);
    error_check(c, "connection done");

    char recv_buffer[BUFFER];

    char request[BUFFER] = "Hi from client\n";

    int snd = send(s, request, strlen(request), 0);

    if (snd > 0)
    {
        printf("message sent\n");
    }
    else
    {
        printf("Error sending message\n");
    }

    int rcv = recv(s, recv_buffer, BUFFER, 0);
    if (rcv > 0)
    {
        printf("Response recieved : %s\n" , recv_buffer);
        recv_buffer[strlen(recv_buffer)] = '\0';
    }
    else
    {
        printf("Error recieving time\n");
    }

    return 0;
}
