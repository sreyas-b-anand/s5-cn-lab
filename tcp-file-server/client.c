#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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

int main()
{

    int s = socket(AF_INET, SOCK_STREAM, 0);
    error_check(s, "socket done");

    struct sockaddr_in client_address;
    client_address.sin_port = htons(8080);
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    socklen_t client_size = sizeof(client_address);

    int c = connect(s, (struct sockaddr *)&client_address, client_size);
    error_check(c, "connection done");

    FileData fd;

    FILE *file;
    file = fopen("source.txt", "r");

    char ch, buffer[BUFFER];
    int i = 0;
    while ((ch = fgetc(file)) != EOF)
    {
        fd.fileContent[i++] = ch;
    }
    fd.fileContent[i+1] = '\0';

    strcpy(fd.fileName, "dest.txt");

    int snd = send(s, &fd, sizeof(FileData), 0);
    if (snd > 0)
        printf("File sent successfully\n");

    int rcv = recv(s, buffer, sizeof(buffer), 0);
    if (rcv > 0)
        printf("ACK received\n");

    fclose(file);
    return 0;
}