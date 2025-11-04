#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

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
}

int main()
{
    int server_socket, client1, client2;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    error_check(server_socket, "Socket created");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
    server_addr.sin_port = htons(PORT);

    error_check(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)), "Bind done");
    error_check(listen(server_socket, 2), "Listening...");

    printf("Waiting for Client 1...\n");
    client1 = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
    error_check(client1, "Client 1 connected");
    printf("Client 1 connected!\n");

    printf("Waiting for Client 2...\n");
    client2 = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
    error_check(client2, "Client 2 connected");
    printf("Client 2 connected!\n");

    int flag = 1;
    while (1)
    {
        int valread;
        // Client1 -> Client2
        if (flag)
        {
            valread = recv(client1, buffer, BUFFER, 0);
            if (valread <= 0)
                break;

            printf("%s\n", buffer);
            buffer[valread] = '\0';
            send(client2, buffer, strlen(buffer), 0);
            printf("sent\n");
            flag = 0;
        }
        else
        {

            // Client2 -> Client1
            valread = recv(client2, buffer, BUFFER, 0);
            if (valread <= 0)
                break;
            buffer[valread] = '\0';
            send(client1, buffer, strlen(buffer), 0);
            flag = 1;
        }
    }

    close(client1);
    close(client2);
    close(server_socket);
    return 0;
}
