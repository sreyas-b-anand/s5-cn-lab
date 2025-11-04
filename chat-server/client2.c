#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define LOCALHOST "127.0.0.1"
#define PORT 8080
#define BUFFER 256

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Socket error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(LOCALHOST);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection error");
        exit(1);
    }

    printf("Client 2 connected! Type messages:\n");
    int flag = 0;
    while (1)
    {

        if (!flag)
        {
            int valread = recv(sock, buffer, BUFFER, 0);
            if (valread <= 0)
                break;
            buffer[valread] = '\0';
            printf("Client1: %s\n", buffer);
            flag = 1;
        }
        else
        {
            printf("Client2: ");
            fgets(buffer, BUFFER, stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            send(sock, buffer, strlen(buffer), 0);
            flag = 0;
        }
    }

    close(sock);
    return 0;
}
