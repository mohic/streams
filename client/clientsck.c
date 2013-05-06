/*
        Maxime LECAT (mlecat)
        Christophe MOHIMONT (cmohimo)

        ce fichier contient les fonctions pour utiliser des sockets clients en IPv4
*/


#include "clientsck.h"

int connexion(char *addresse, unsigned short port)
{
	// crée un socket tcp
	int sck = socket(AF_INET, SOCK_STREAM, 0);

	if (sck == -1) {
		perror("socket");
		return -1;
	}
	
	// configuration du socket
	SOCKADDR_IN sin;

	sin.sin_addr.s_addr = inet_addr(addresse); // adresse du serveur
	sin.sin_family = AF_INET; // ipv4
	sin.sin_port = htons(port); // port

	// connexion au serveur
	if (connect(sck, (SOCKADDR *)&sin, sizeof(sin))) {
		if (errno == 111) // pas de serveur
			printf("Aucun serveur present\n");
		else
			perror("connect");

		fermerSocket(sck);
		return -1;
	}

	// retourne le socket ouvert
	return sck;
}

int fermerSocket(int socket)
{
	int c = close(socket);

	if (c == -1)
		perror("closesocket");

	return c;
}

int envoyerMessage(int socket, char *message)
{
	int result = send(socket, message, TAILLE_BUFFER, 0);

	if (result == -1)
	{
		perror("send");
		return -1;
	}

	return result;
}

int recevoirMessage(int socket, char *message)
{
	int result = recv(socket, message, TAILLE_BUFFER, 0);

	if (result == -1) {
		perror("recv");
		return -1;
	}

	return result;
}

