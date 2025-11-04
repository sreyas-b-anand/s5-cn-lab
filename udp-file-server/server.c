#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080
#define LOCALHOST "127.0.0.1"
#define BUFFER 256

void error_check(int val, char buffer[]){
    if(val < 0){
        printf("Error \n");
        exit(1);
    }else printf("%s\n" , buffer);
}

struct sockaddr_in create_addr(){
    struct sockaddr_in server_address;
    server_address.sin_port=htons(PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(LOCALHOST);

    return server_address;
}

int main(){

    char ch , fileContent[BUFFER] , recv_buffer[BUFFER];
    int i=0;

    int s = socket(AF_INET , SOCK_DGRAM , 0);
    error_check(s , "socket done");

    struct sockaddr_in server_address = create_addr();
    socklen_t server_size = sizeof(server_address);

    int b=bind(s , (struct sockaddr *)&server_address , server_size); 
    error_check(b , "binding done");

    struct sockaddr_in client_address;
    socklen_t client_size = sizeof(client_address);

    int rcv = recvfrom(s , recv_buffer , BUFFER ,0 , (struct sockaddr *)&client_address , &client_size);
    if(rcv > 0){
        printf("Request received : %s\n" , recv_buffer);
    }else{
        printf("Error receiving request\n");
    }

    FILE *file = fopen("sample.txt" , "r");
    if(!file) return 1;

   
    while((ch = fgetc(file)) != EOF && i < BUFFER - 1){
        fileContent[i++] = ch;
    }

    printf("file content : %s\n" , fileContent);

    int snd = sendto(s , fileContent , strlen(fileContent) , 0 ,(struct sockaddr *)&client_address , client_size );
    if(snd > 0){
        printf("Response sent\n");
    }else{
        printf("Error sending response\n");
    }

    fclose(file);

    return 0;
}