#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct s_clients
{
	int id;
	char msg[100000];
} t_clients;

t_clients clients[1024];
fd_set readSockets, writeSockets, activeSockets;
int maxSocket = 0, next_id = 0;
char bufferRead[120000], bufferWrite[120000];

static void fatalError(char *str)
{
	if (str)
		write(2, str, strlen(str));
	else
		write(2, "Fatal error\n", 12);
	exit(1);
}

static void sendAll(int currentClient)
{
	for (int count = 0; count <= maxSocket; count++)
		if (FD_ISSET(count, &writeSockets) && count != currentClient)
			send(count, bufferWrite, strlen(bufferWrite), 0);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		fatalError("Wrong number of arguments\n");

	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
		fatalError(NULL);

	bzero(&clients, sizeof(clients));
	FD_ZERO(&activeSockets);
	maxSocket = serverSocket;
	FD_SET(serverSocket, &activeSockets);

	struct sockaddr_in serverAddress;
	socklen_t len;
	bzero(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(2130706433);
	serverAddress.sin_port = htons(atoi(argv[1]));

	if (bind(serverSocket, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		fatalError(NULL);
	if (listen(serverSocket, 10) < 0)
		fatalError(NULL);

	while (1)
	{
		readSockets = writeSockets = activeSockets;
		if (select(maxSocket + 1, &readSockets, &writeSockets, NULL, NULL) < 0)
			continue;

		for (int socketId = 0; socketId <= maxSocket; socketId++)
		{
			if (FD_ISSET(socketId, &readSockets) && socketId == serverSocket)
			{
				int clientSocket = accept(serverSocket, (struct sockaddr *)&serverAddress, &len);
				if (clientSocket < 0)
					continue;

				maxSocket = (clientSocket > maxSocket) ? clientSocket : maxSocket;
				clients[clientSocket].id = next_id++;
				FD_SET(clientSocket, &activeSockets);
				sprintf(bufferWrite, "server: client %d just arrived\n", clients[clientSocket].id);
				sendAll(clientSocket);
				break;
			}
			if (FD_ISSET(socketId, &readSockets) && socketId != serverSocket)
			{
				int bytesRead = recv(socketId, bufferRead, 65536, 0);

				if (bytesRead <= 0)
				{
					sprintf(bufferWrite, "server: client %d just left\n", clients[socketId].id);
					sendAll(socketId);
					FD_CLR(socketId, &activeSockets);
					close(socketId);
					break;
				}
				else
				{
					for (int pos = 0, count = strlen(clients[socketId].msg); pos < bytesRead; pos++, count++)
					{
						clients[socketId].msg[count] = bufferRead[pos];
						if (clients[socketId].msg[count] == '\n')
						{
							clients[socketId].msg[count] = '\0';
							sprintf(bufferWrite, "client %d: %s\n", clients[socketId].id, clients[socketId].msg);
							sendAll(socketId);
							bzero(&clients[socketId].msg, strlen(clients[socketId].msg));
							count = -1;
						}
					}
					break;
				}
			}
		}
	}
	return (0);
}