#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	int socket_desc, connect_desc;										//	server sock, user sock
	char data[2048];													//	array data for data transmission
	WSADATA wsa_data;													//	struct wincock dll
	fd_set read_fd;														//	struct for winsock select function
	struct timeval time_desc;											//	time struct for select function
	struct sockaddr_in server_addr;										//	server address struct

	memset(data, '\0', sizeof(data));
	data[0] = 0;														//	init first elelemt to 0
	time_desc.tv_sec = 1;												//	init time to 1 sec
	server_addr.sin_family = AF_INET; 
	server_addr.sin_port = htons(2000);									//	init port 2000
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.S_un.S_addr);	//	init server ip adress 127.0.0.1

	//	init winsock dll
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("DEBUG: WSAStartup Erro\n");
		return -1;
	}
	printf("DEBUG: WSAStartup Success\n");

	//	create client an endpoint for communication
	socket_desc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_desc == INVALID_SOCKET)
	{
		printf("DEBUG: Socket Error\n");
		return socket_desc;
	}
	printf("DEBUG: Socket Success\n");

	//	initiate a connection on a socket
	connect_desc = connect(socket_desc, &server_addr, sizeof(server_addr));
	if (connect_desc < 0)
	{
		printf("DEBUG: Connect Error\n");
		return connect_desc;
	}
	printf("DEBUG: Connect Success\n");

	//	while loop
	while (1)
	{
		//	clean read_fd
		FD_ZERO(&read_fd);

		//	set some value to read_fd
		FD_SET(socket_desc, &read_fd);

		//	call monitors activity on a set of sockets / 1 sec wait and continue
		select(socket_desc + 1, &read_fd, NULL, NULL, &time_desc);

		//	Checks the console for keyboard input.
		while (_kbhit())
		{
			printf("Enter message: ");
			fgets(data, sizeof(data), stdin);					//	get data from input
			send(socket_desc, data, sizeof(data), 1);			//	send data to server
			memset(data, '\0', sizeof(data));
			data[0] = 0;
		}

		//	return a value for the file descriptor
		if (FD_ISSET(socket_desc, &read_fd))
		{
			//	check for the receive data
			if (recv(socket_desc, data, sizeof(data), 0) > 0)
			{
				if (data[0] != 0)
				{
					printf("Message from users: %s", data);
					memset(data, '\0', sizeof(data));
					data[0] = 0;
				}
			}
		}
	}

	return 0;
}