#include <stdlib.h>
#include <stdio.h>
#include <conio.h>



#include <winsock2.h>
#include <ws2tcpip.h>


#pragma comment(lib, "Ws2_32.lib")





int main()
{
	WSADATA wsa_data;

	fd_set read_fd;

	struct timeval time_desc;
	time_desc.tv_sec = 1;

	char WSAStartup_respons = WSAStartup(MAKEWORD(2, 2), &wsa_data);

	if (WSAStartup_respons != 0)
	{
		printf("DEBUG: WSAStartup Erro\n");
		return WSAStartup_respons;
	}
	printf("DEBUG: WSAStartup Success\n");

	int socket_desc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socket_desc == INVALID_SOCKET)
	{
		printf("DEBUG: Socket Error\n");
		return socket_desc;
	}

	printf("DEBUG: Socket Success\n");


	struct sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2000);
	inet_pton(AF_INET, "85.28.97.22", &server_addr.sin_addr.S_un.S_addr);
	char connect_desc = connect(socket_desc, &server_addr, sizeof(server_addr));

	if (connect_desc < 0)
	{
		printf("DEBUG: Connect Error\n");
		return connect_desc;
	}

	printf("DEBUG: Connect Success\n");

	char data[2048];
	memset(data, '\0', sizeof(data));
	data[0] = 0;

	while (1)
	{
		FD_ZERO(&read_fd);
		FD_SET(socket_desc, &read_fd);

		select(socket_desc + 1, &read_fd, NULL, NULL, &time_desc);

		while (_kbhit())
		{
			printf("Enter message: ");
			fgets(data, sizeof(data), stdin);
			send(socket_desc, data, sizeof(data), 1);
			memset(data, '\0', sizeof(data));
			data[0] = 0;
		}

		if (FD_ISSET(socket_desc, &read_fd))
		{
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