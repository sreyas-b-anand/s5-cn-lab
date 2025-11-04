#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define LOCALHOST "127.0.0.1"
#define PORT 8080
#define BUFFER 256

void err_check(int val, char message[])
{
    if (val < 0)
    {
        printf("Error");
        exit(1);
    }
    else
    {
        printf("%s", message);
    }
}

int main()
{

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    err_check(s, "socket done\n");

    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(PORT);
    client_address.sin_addr.s_addr = inet_addr(LOCALHOST);

    socklen_t len1 = sizeof(client_address);

    char buffer[] = "Hi ..this is client request\n";
    int n = sendto(s, buffer, strlen(buffer), 0, (struct sockaddr *)&client_address, len1);
    if (n > 0)
    {
        printf("Message sent\n");
    }
    else
    {
        printf("error sending message\n");
    }

    char recv_buffer[BUFFER];

    int r = recvfrom(s, recv_buffer, BUFFER, 0, (struct sockaddr *)&client_address, &len1);
    if (r > 0)
    {
        printf("Message received : %s\n", recv_buffer);
    }
    else
    {
        printf("error receiving message\n");
    }

    return 0;
}