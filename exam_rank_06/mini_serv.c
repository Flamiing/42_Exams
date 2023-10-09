/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_serv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaaouam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 12:58:38 by alaaouam          #+#    #+#             */
/*   Updated: 2023/10/09 17:55:29 by alaaouam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>

static void fatalError(int socket)
{
	if (socket != -1)
		close(socket);
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
	int clientSockets[128];
	int next_id = 0;
	int connectedClients = 0;
	int clients[132];
	fd_set activeSockets, readySockets;
	char buffer[200000];
	char msgBuffer[200000];
	
	// Create server socket and setup address:
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
		fatalError(-1);
	struct sockaddr_in serverAddress = {0}; // Structure to hold server address
	serverAddress.sin_family = AF_INET; // Address familly to IPv4
	serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Set ip address to localhost
	serverAddress.sin_port = htons(atoi(argv[1])); // Set port

	// Bind server socket to address:
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		fatalError(serverSocket);

	// Listens to upcomming connections:
	if (listen(serverSocket, 128) < 0)
		fatalError(serverSocket);
	
	// Initialise the active sockets set:
	FD_ZERO(&activeSockets); // Clear set of active sockets
	FD_SET(serverSocket, &activeSockets); // Add server socket to the set
	int maxSocket = serverSocket; // Track the maximum socker descriptor

	while (1)
	{
		// Wait for activity in the sockets:
		readySockets = activeSockets;
		if (select(maxSocket + 1, &readySockets, NULL, NULL, NULL) < 0)
			fatalError(-1);

		// Check each socket for activity:
		for (int socketId = 0; socketId <= maxSocket; socketId++)
		{
			if (FD_ISSET(socketId, &readySockets))
			{
				// New client connection: 
				int currentClient = socketId;
				if (socketId == serverSocket)
				{
					int clientSocket = accept(serverSocket, NULL, NULL); // Accept new client connection
					if (clientSocket < 0)
						fatalError(-1);
					
					FD_SET(clientSocket, &activeSockets); // Add new client to the active set of sockets
					maxSocket = (clientSocket > maxSocket) ? clientSocket : maxSocket; // Update the max socket descriptor

					sprintf(msgBuffer, "server: client %d just arrived\n", next_id);
					clients[clientSocket] = next_id;
					clientSockets[next_id++] = clientSocket; // Add client to the list of clients
					connectedClients++;
					broadcast(clientSockets, clientSocket, connectedClients, msgBuffer); // Broadcast the msg to all clients
				}
				else
				{
					int bytesRead = recv(socketId, buffer, sizeof(buffer) - 1, 0);
					
					if (bytesRead <= 0)
					{
						// Client disconnected:
						sprintf(msgBuffer, "server: client %d just left\n", clients[currentClient]); // Prepare info for the rest of the clients
						broadcast(clientSockets, socketId, connectedClients, msgBuffer); // Broadcast msg to the clients
						close(socketId); // Close fd of the client disconnected
						FD_CLR(socketId, &activeSockets); // Removes the client from the set of active clients
						connectedClients--;
					}
					else
					{
						buffer[bytesRead] = '\0';
						sprintf(msgBuffer, "client: %d: %s", clients[currentClient], buffer);
						broadcast(clientSockets, socketId, connectedClients, msgBuffer);
					}
				}
			}
		}
	}
	return (0);
}