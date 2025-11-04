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

struct sockaddr_in create_addr()
{
    struct sockaddr_in sock_address;
    sock_address.sin_family = AF_INET;
    sock_address.sin_addr.s_addr = inet_addr(LOCALHOST);
    sock_address.sin_port = htons(PORT);

    return sock_address;
}
int main()
{
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    err_check(s, "socket done\n");

    struct sockaddr_in server_address = create_addr();
    socklen_t len1 = sizeof(server_address);

    int b = bind(s, (struct sockaddr *)&server_address, len1);
    err_check(b, "bind done\n");

    struct sockaddr_in client_address;
    socklen_t len2 = sizeof(client_address);

    char recv_buffer[BUFFER];

    char snd_msg[] = "Hi ..this is server response\n";

    int n = recvfrom(s, recv_buffer, BUFFER, 0, (struct sockaddr *)&client_address, &len2);
    if (n > 0)
    {
        printf("message received : %s \n", recv_buffer);
    }
    else
        printf("error receiving message\n");

    int snd = sendto(s, snd_msg, strlen(snd_msg), 0, (struct sockaddr *)&client_address, len2);

    if (snd > 0)
    {
        printf("message sent\n");
    }
    else
        printf("error sending message\n");

    return 0;
}