/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_serv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaaouam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 12:58:38 by alaaouam          #+#    #+#             */
/*   Updated: 2023/10/14 14:58:00 by alaaouam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>

static void closeSockets(int *sockets)
{
	int count = 0;
	while (count < 128)
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

static void broadcast(int *clientSockets, int currentClient, int size, char *buffer)
{
	for (int count = 0; count < size; count++) 
	{
		if (clientSockets[count] != currentClient)
			send(clientSockets[count], buffer, strlen(buffer), 0);
	}	
}

int main(int argc, char ** argv)
{
	// Manage wrong number of arguments:
	if (argc != 2)
	{
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}

	// Clients, fd sets and buffer declarations:
	int clientSockets[128] = {-1};
	int next_id = 0; // Keeps track of the next id for the next client
	int connectedClients = 0; // Keeps track of the connected clients to the server
	int clientsIds[132]; // 132 to leave space for the first 3FDs: 0, 1, 2
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
					connectedClients++;
					broadcast(clientSockets, clientSocket, connectedClients, msgBuffer); // Broadcast the msg to all clients
				}
				else
				{
					int bytesRead = recv(socketId, buffer, sizeof(buffer) - 1, 0); // Reads the msg sended by the client
					
					if (bytesRead <= 0) // Checks if the client is still connected
					{
						// Client disconnected:
						sprintf(msgBuffer, "server: client %d just left\n", clientsIds[socketId]); // Prepare info for the rest of the clients
						broadcast(clientSockets, socketId, connectedClients, msgBuffer); // Broadcast msg to the clients
						close(socketId); // Close fd of the client disconnected
						FD_CLR(socketId, &activeSockets); // Removes the client from the set of active clients
						connectedClients--;
					}
					else // If is connected, the message is sended to the rest of the clients
					{
						buffer[bytesRead] = '\0';
						sprintf(msgBuffer, "client: %d: %s", clientsIds[socketId], buffer);
						broadcast(clientSockets, socketId, connectedClients, msgBuffer);
					}
				}
			}
		}
	}
	return (0);
}