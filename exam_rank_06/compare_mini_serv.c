#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>

static void closeSockets(int *sockets)
{
	int count = 0;
	while (count < 4000)
	{
		if (sockets[count] != -1)
			close(sockets[count]);
		count++;
	}
}

static void fatalError(int serverSocket, int *sockets)
{
	if (sockets != NULL)
		closeSockets(sockets);
	if (serverSocket != -1)
		close(serverSocket);
	write(2, "Fatal error\n", 12);
	exit(1);
}

static void broadcast(int *clientSockets, int currentClient, char *buffer)
{
	for (int count = 0; count < 4000; count++)
	{
		if (clientSockets[count] != currentClient)
			send(clientSockets[count], buffer, strlen(buffer), 0);
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}

	int clientSockets[4000] = {-1};
	int next_id = 0;
	int clientsIds[4003];
	fd_set activeSockets, readySockets;
	char buffer[200000];
	char msgBuffer[200050];

	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
		fatalError(-1, NULL);
	struct sockaddr_in serverAddress = {0};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	serverAddress.sin_port = htons(atoi(argv[1]));

	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		fatalError(serverSocket, NULL);
	if (listen(serverSocket, 128) < 0)
		fatalError(serverSocket, NULL);

	FD_ZERO(&activeSockets);
	FD_SET(serverSocket, &activeSockets);
	int maxSocket = serverSocket;

	while (1)
	{
		readySockets = activeSockets;
		if (select(maxSocket + 1, &readySockets, NULL, NULL, NULL) < 0)
			continue;

		for (int socketId = 0; socketId <= maxSocket; socketId++)
		{
			if (FD_ISSET(socketId, &readySockets))
			{
				if (socketId == serverSocket)
				{
					int clientSocket = accept(serverSocket, NULL, NULL);
					if (clientSocket < 0)
						fatalError(serverSocket, clientSockets);

					FD_SET(clientSocket, &activeSockets);
					maxSocket = (clientSocket > maxSocket) ? clientSocket : maxSocket;

					sprintf(msgBuffer, "server: client %d just arrived\n", next_id);
					clientsIds[clientSocket] = next_id;
					clientSockets[next_id++] = clientSocket;
					broadcast(clientSockets, clientSocket, msgBuffer);
				}
				else
				{
					int bytesRead = recv(socketId, buffer, sizeof(buffer) - 1, 0);

					if (bytesRead <= 0)
					{
						sprintf(msgBuffer, "server: client %d just left\n", clientsIds[socketId]);
						broadcast(clientSockets, socketId, msgBuffer);
						close(socketId);
						FD_CLR(socketId, &activeSockets);
					}
					else
					{
						buffer[bytesRead] = '\0';
						sprintf(msgBuffer, "client %d: %s", clientsIds[socketId], buffer);
						broadcast(clientSockets, socketId, msgBuffer);
					}
				}
			}
		}
	}
	return (0);
}