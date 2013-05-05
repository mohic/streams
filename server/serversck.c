/*******************************************************\
*       Maxime LECAT (mlecat)                           *
*       Christophe MOHIMONT (cmohimo)                   *
*                                                       *
*       ce fichier contient les fonctions pour utiliser *
*	un socket IPv4 en ecoute                        *
\*******************************************************/

#include "serversck.h"

int ouvrirSocket(unsigned short port)
{
	// crée un socket tcp
	int sck = socket(AF_INET, SOCK_STREAM, 0);

	if (sck == -1) {
		perror("socket");
		return -1;
	}
	
	// configuration du socket
	SOCKADDR_IN sin;

	sin.sin_addr.s_addr = inet_addr("127.0.0.1"); // adresse autorisée
	sin.sin_family = AF_INET; // ipv4
	sin.sin_port = htons(port); // port

	// envoie la configuration au socket
	if (bind(sck, (SOCKADDR *)&sin, sizeof(sin))) {
		perror("bind");
		fermerSocket(sck);
		return -1;
	}

	// met le serveur en écoute
	if (listen(sck, 5) == -1) {
		perror("listen");
		fermerSocket(sck);
		return -1;
	}

	// retourne le socket ouvert
	return sck;
}

int accepterClient(int socket) {
	SOCKADDR_IN sin;
	int structsize = sizeof(sin);

	int sckClient = accept(socket, (SOCKADDR *)&sin, &structsize);
	
	if (sckClient == -1) {
		perror("accept");
		return -1;
	}

	printf("Le client %d vient de se connecter\n", sckClient);

	return sckClient;
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
	int result = send(socket, message, strlen(message) + 1, 0);

	if (result == -1) {
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

