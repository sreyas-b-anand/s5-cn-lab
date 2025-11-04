#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define LOCALHOST "127.0.0.1"
#define PORT 8080
#define BUFFER 256

struct sockaddr_in create_socket_address()
{
	struct sockaddr_in socket_address;
	socket_address.sin_family = AF_INET;
	socket_address.sin_addr.s_addr = inet_addr(LOCALHOST);
	socket_address.sin_port = htons(PORT);
	return socket_address;
}

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

	struct sockaddr_in server_address = create_socket_address();
	socklen_t len1 = sizeof(server_address);

	int b = bind(s, (struct sockaddr *)&server_address, len1);
	error_check(b, "bind done");

	int l = listen(s, 5);
	error_check(l, "listening...");

	struct sockaddr_in client_address;
	socklen_t len2 = sizeof(client_address);

	int new_socket = accept(s, (struct sockaddr *)&client_address, &len2);
	error_check(new_socket, "new socket done");

	char recv_buffer[BUFFER];

	int rcv = recv(new_socket, recv_buffer, BUFFER, 0);
	if (rcv > 0)
	{
		recv_buffer[strlen(recv_buffer)] = '\0';
		printf("Request recieved: %s\n", recv_buffer);
	}
	

	int snd = send(new_socket, "Hi from server", strlen("Hi from server"), 0);
	if (snd > 0)
	{
		printf("response sent\n");
	}
	

	return 0;
}
