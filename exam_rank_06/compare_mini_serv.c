#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>

static void closeSockets(int *sockets)
{
	int socket = 0;
	while (socket < 1024)
	{
		if (sockets[socket] != -1)
			close(socket);
		socket++;
	}
}

fd_set activeSockets, readySockets, writeSockets;

static void fatalError(int serverSocket, int *sockets)
{
	if (sockets != NULL)
		closeSockets(sockets);
	if (serverSocket != -1)
		close(serverSocket);
	write(2, "Fatal error\n", 12);
	exit(1);
}

static void broadcast(int maxSocket, int currentClient, char *buffer)
{
	for (int count = 3; count <= maxSocket; count++)
	{
		if (FD_ISSET(count, &writeSockets) && count != currentClient)
			send(count, buffer, strlen(buffer), 0);
	}
}

static void sendMsg(char *buffer, int bytesRead, int maxSocket, int socketId, int id)
{
	char msg[200050];
	char temp[200050];
	int count = 0;
	int pos = 0;

	while (count < bytesRead)
	{
		pos = 0;
		while (buffer[count] && buffer[count] != '\n')
		{
			temp[pos] = buffer[count];
			pos++;
			count++;
		}
		count++;
		temp[pos] = '\0';
		sprintf(msg, "client %d: %s\n", id, temp);
		broadcast(maxSocket, socketId, msg);
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}

	int clientSockets[1024] = {-1};
	int next_id = 0;
	char buffer[200000];
	char msgBuffer[200050];
	socklen_t len;

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
		writeSockets = activeSockets;
		if (select(maxSocket + 1, &readySockets, &writeSockets, NULL, NULL) < 0)
			continue;

		for (int socketId = 3; socketId <= maxSocket; socketId++)
		{
			if (FD_ISSET(socketId, &readySockets))
			{
				if (socketId == serverSocket)
				{
					int clientSocket = accept(serverSocket, (struct sockaddr *)&serverAddress, &len);
					if (clientSocket < 0)
						fatalError(serverSocket, clientSockets);

					FD_SET(clientSocket, &activeSockets);
					maxSocket = (clientSocket > maxSocket) ? clientSocket : maxSocket;

					sprintf(msgBuffer, "server: client %d just arrived\n", next_id);
					clientSockets[clientSocket] = next_id++;
					broadcast(maxSocket, clientSocket, msgBuffer);
				}
				else
				{
					int bytesRead = recv(socketId, buffer, sizeof(buffer) - 1, 0);

					if (bytesRead <= 0)
					{
						sprintf(msgBuffer, "server: client %d just left\n", clientSockets[socketId]);
						broadcast(maxSocket, socketId, msgBuffer);
						close(socketId);
						FD_CLR(socketId, &activeSockets);
						clientSockets[socketId] = -1;
					}
					else
						sendMsg(buffer, bytesRead, maxSocket, socketId, clientSockets[socketId]);
				}
			}
		}
	}
	return (0);
}