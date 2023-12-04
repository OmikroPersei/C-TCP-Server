#include <winsock2.h>
#include <ws2tcpip.h>
#include "Server_Func.h"
#pragma comment(lib, "Ws2_32.lib")

unsigned int* client_sock, client_temp;			//	client sock array, temp client sock array
unsigned int client_count = 0;					//	client count on the server

//	malloc function 
void malloc_array(unsigned int* new_client_sock, unsigned int new_client_count)
{
	new_client_sock = MALLOC_INT(new_client_count);
}

//	add an element to the client_sock array
void add_element()
{
	if (client_count > 0)
	{
		malloc_array(client_temp, client_count);
		if (client_temp)
		{
			MEMCPY(client_temp, client_sock);
			FREE(client_sock);
		}

		malloc_array(client_sock, client_count + 1);
		if (client_sock && client_temp)
		{
			MEMCPY(client_sock, client_temp);
			FREE(client_temp);
		}
	}
}

//	remove an element from the client_sock array
void remove_element()
{
	if (client_count > 0)
	{
		malloc_array(client_temp, client_count);
		if (client_temp)
		{
			MEMCPY(client_temp, client_sock);
			FREE(client_sock);
		}

		malloc_array(client_sock, client_count);
		if (client_sock && client_temp)
		{
			MEMCPY(client_sock, client_temp);
			FREE(client_temp);
		}
	}
}

int main()
{
	int socket_desc, client_size, count, iteration;										//	server socket, client data size, iterators
	char data[2048], ipinput[INET_ADDRSTRLEN];											//	data array, ip address array
	struct timeval time_desc;															//	time struct
	struct sockaddr_in server_addr, temp_addr;											//	server address struct
	fd_set read_fd;																		//	struct for winsock select function
	WSADATA wsa_data;																	//	struct wincock dll

	client_size = sizeof(temp_addr);													//	init client_size data
	time_desc.tv_sec = 1;																//	init time to 1 sec
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2000);													//	init port 2000
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.S_un.S_addr);					//	init server ip adress 127.0.0.1
	memset(data, '\0', sizeof(data));
	client_sock = malloc(sizeof(unsigned int));											// init one array element for client_sock

	//	init winsock dll
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("DEBUG: WSAStartup Erro\n");
		return -1;
	}
	printf("DEBUG: WSAStartup Success\n");

	//	create server an endpoint for communication
	socket_desc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_desc == INVALID_SOCKET)
	{
		printf("DEBUG: Socket Error\n");
		return socket_desc;
	}
	printf("DEBUG: Socket Success\n");

	//	binds a unique local name to the socket with descriptor socket
	if (bind(socket_desc, &server_addr, sizeof(server_addr)) < 0)
	{
		printf("DEBUG: Bind Error\n");
		return -1;
	}
	printf("DEBUG: Bind Success\n");

	//	prepare the server for incoming client requests
	if (listen(socket_desc, 2) < 0)
	{
		printf("DEBUG: Listen Server Error\n");
		return -1;
	}
	printf("DEBUG: Listen Server Success\n");

	//	while loop
	while (1)
	{
		//	clean read_fd
		FD_ZERO(&read_fd);

		//	set some value to read_fd
		FD_SET(socket_desc, &read_fd);

		//	set existe client on server to read_fd
		for (iteration = 0; iteration < client_count; iteration++)
		{
			if (client_sock[iteration] > 0)
			{
				FD_SET(client_sock[iteration], &read_fd);
			}
		}

		//	call monitors activity on a set of sockets / 1 sec wait and continue
		if (select(socket_desc + 1, &read_fd, NULL, NULL, &time_desc) < 0)
		{
			printf("DEBUG: Select Server Error\n");
		}

		//	return a value for the file descriptor
		if (FD_ISSET(socket_desc, &read_fd))
		{
			int temp_sock = accept(socket_desc, &temp_addr, &client_size);

			if (temp_sock < 0)
			{
				return 1;
			}
			else
				printf("Client connected at IP: %s and port: %i\n", inet_ntop(AF_INET, &(temp_addr.sin_addr), ipinput, INET_ADDRSTRLEN), ntohs(temp_addr.sin_port));

			add_element();
			client_sock[client_count] = temp_sock;
			client_count++;
		}

		//	check if there are any changes in the client sock
		for (iteration = 0; iteration < client_count; iteration++)
		{
	
			if (FD_ISSET(client_sock[iteration], &read_fd))
			{
				//	if data change
				if (recv(client_sock[iteration], data, sizeof(data), 0) > 0)
				{
					for (count = 0; count < client_count; count++)
					{
						if (client_sock[count] > 0 && client_sock[count] != client_sock[iteration])
						{
							send(client_sock[count], data, sizeof(data), 0);
						}
					}
				}
				else //	else: if the client is disconnected
				{
					printf("Get out\n");
					closesocket(client_sock[iteration]);			// close client sock
					client_sock[iteration] = 0;						// init old client to 0 value
					client_count--;
					bubble(client_sock, client_count + 1);			// bubble sort algorithm
					remove_element();								// remove 0 value from array
				}
				memset(data, '\0', sizeof(data));
			}
		}
	}
	closesocket(socket_desc);										// close server sock
	return 0;
}