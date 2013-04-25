/*
	Maxime LECAT (mlecat)
	Christophe MOHIMONT (cmohimo)

	ce fichier contient la fonction main démarrant le programme client
*/
#include "clientsck.h"

//TODO remplacer printf par write

int main (int argc, char* argv[])
{
	// vérification # arguments
	if (argc <= 1 || argc >= 3) {
		printf("Usage: %s port [fichierErreur]\n", argv[0]);
		return 1;
	}

	// obtenir le port
	char *partieEnTrop;
	int port = strtol(argv[1], &partieEnTrop, 10);

	if (port < 1 || port > 65535 || partieEnTrop[0] != '\0') {
		printf("Le port doit être entre 1 et 65535\n");
		return 1;
	}

	printf("Connexion au serveur à l'adresse %s:%d\n", "127.0.0.1", port);

	// connexion au serveur
	int sck = connexion("127.0.0.1", (unsigned short)port);

	if (sck == -1) {
		return 1;
	}

	printf("Connecté au serveur à l'adresse %s:%d\n", "127.0.0.1", port);

	// choisir un nom
	char *msgNom = "Entrez votre nom : ";
	write(STDIN_FILENO, msgNom, strlen(msgNom) + 1);

	char nom[21] = {'\0'};
	read(STDIN_FILENO, nom, sizeof(nom)-1);

	write(STDIN_FILENO, "\n", 2);

	// formater le message d'inscription
	char msg[sizeof(nom) + 2];
	sprintf(msg, "1=%s", nom);

	if (envoyerMessage(sck, msg) == -1)
		printf("erreur envois\n");
	else
		printf("Message d'inscription envoye\n");

	// boucle de lecture / envois de messages
	//while (1) {
		//TODO créer un fichier contenant la logique du jeu
		char message[201] = {'\0'};

		//printf("lecture message\n");
		if (recevoirMessage(sck, message, sizeof(message)-1) != -1) {
                        if (strncmp(message, "1=0", 3))
				printf("Inscription acceptee\n");
			else
				printf("Inscription refusee\n");
                }

                printf("\n");
	//}

	// ferneture de la socket
	return fermerSocket(sck);
}
