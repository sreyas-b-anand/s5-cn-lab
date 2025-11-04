#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080
#define LOCALHOST "127.0.0.1"
#define BUFFER 256

typedef struct
{
    char fileName[20];
    char fileContent[100];
} FileData;

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

struct sockaddr_in create_addr()
{
    struct sockaddr_in server_address;
    server_address.sin_port = htons(PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(LOCALHOST);

    return server_address;
}

int main()
{

    FileData fd;

    int s = socket(AF_INET, SOCK_STREAM, 0);
    error_check(s, "socket done");

    struct sockaddr_in server_address = create_addr();
    socklen_t server_size = sizeof(server_address);

    int b = bind(s, (struct sockaddr *)&server_address, server_size);
    error_check(b, "bind done");

    int l = listen(s, 5);
    error_check(l, "listening...");

    struct sockaddr_in client_address;
    socklen_t client_size = sizeof(client_address);

    int a = accept(s, (struct sockaddr *)&client_address, &client_size);
    error_check(a, "accepted");

    int rcv = recv(a, &fd, sizeof(FileData), 0);
    printf("F C : %s\n" , fd.fileContent);
    FILE *file;
    if (rcv > 0)
    {
        printf("Received File Name: %s\n", fd.fileName);
        printf("File Content: %s\n", fd.fileContent);

        FILE *file = fopen(fd.fileName, "w");
        if (!file)
        {
            perror("File open error");
            return 1;
        }

        size_t written = fwrite(fd.fileContent, sizeof(char), strlen(fd.fileContent), file);
        if (written > 0)
            printf("File content written to %s\n", fd.fileName);
        else
            printf("Can't write to file\n");

        fclose(file);

        int snd = send(a, "ACK", sizeof("ACK"), 0);
        if (snd > 0)
            printf("ACK sent\n");
    }

    return 0;
}