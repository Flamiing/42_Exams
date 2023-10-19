/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_serv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaaouam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 12:58:38 by alaaouam          #+#    #+#             */
/*   Updated: 2023/10/19 22:55:21 by alaaouam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (clientSockets[count] != currentClient && clientSockets[count] != -1)
			send(clientSockets[count], buffer, strlen(buffer), 0);
	}
}

static void sendMsg(char *buffer, int bytesRead, int *clientSockets, int socketId, int id)
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
		broadcast(clientSockets, socketId, msg);
	}
}

int main(int argc, char **argv)
{
	// Manage wrong number of arguments:
	if (argc != 2)
	{
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}

	// Clients, fd sets and buffer declarations:
	int clientSockets[4000] = {-1};
	int next_id = 0; // Keeps track of the next id for the next client
	int clientsIds[4003]; // 4003 to leave space for the first 3FDs: 0, 1, 2
	fd_set activeSockets, readySockets;
	char buffer[200000]; // Buffer to read with recv
	char msgBuffer[200050]; // Buffer to save and send the msg

	// Create server socket and setup address:
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
		fatalError(-1, NULL);
	struct sockaddr_in serverAddress = {0}; // Structure to hold server address
	serverAddress.sin_family = AF_INET; // Address familly to IPv4
	serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Set ip address to localhost
	serverAddress.sin_port = htons(atoi(argv[1])); // Set port

	// Bind server socket to address:
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		fatalError(serverSocket, NULL);

	// Listens to upcomming connections:
	if (listen(serverSocket, 128) < 0)
		fatalError(serverSocket, NULL);

	// Initialise the active sockets set:
	FD_ZERO(&activeSockets); // Clear set of active sockets
	FD_SET(serverSocket, &activeSockets); // Add server socket to the set
	int maxSocket = serverSocket; // Track the maximum socker descriptor

	while (1)
	{
		// Wait for activity in the sockets:
		readySockets = activeSockets;
		if (select(maxSocket + 1, &readySockets, NULL, NULL, NULL) < 0)
			continue;

		// Check each socket for activity:
		for (int socketId = 0; socketId <= maxSocket; socketId++)
		{
			if (FD_ISSET(socketId, &readySockets))
			{
				// New client connection: 
				if (socketId == serverSocket)
				{
					int clientSocket = accept(serverSocket, NULL, NULL); // Accept new client connection
					if (clientSocket < 0)
						fatalError(serverSocket, clientSockets);

					FD_SET(clientSocket, &activeSockets); // Add new client to the active set of sockets
					maxSocket = (clientSocket > maxSocket) ? clientSocket : maxSocket; // Update the max socket descriptor

					sprintf(msgBuffer, "server: client %d just arrived\n", next_id);
					clientsIds[clientSocket] = next_id; // Map like array to save (key: client socket, value: current id)
					clientSockets[next_id++] = clientSocket; // Add client to the list of clients
					broadcast(clientSockets, clientSocket, msgBuffer); // Broadcast the msg to all clients
				}
				else
				{
					int bytesRead = recv(socketId, buffer, sizeof(buffer) - 1, 0); // Reads the msg sended by the client

					if (bytesRead <= 0) // Checks if the client is still connected
					{
						// Client disconnected:
						sprintf(msgBuffer, "server: client %d just left\n", clientsIds[socketId]); // Prepare info for the rest of the clients
						broadcast(clientSockets, socketId, msgBuffer); // Broadcast msg to the clients
						close(socketId); // Close fd of the client disconnected
						FD_CLR(socketId, &activeSockets); // Removes the client from the set of active clients
						clientSockets[clientsIds[socketId]] = -1;
					}
					else // If is connected, the message is sended to the rest of the clients
						sendMsg(buffer, bytesRead, clientSockets, socketId, clientsIds[socketId]);
				}
			}
		}
	}
	return (0);
}