/*
	Maxime LECAT (mlecat)
	Christophe MOHIMONT (cmohimo)

	ce fichier contient la fonction main démarrant le programme client
*/
#include "clientsck.h"
#include "../common/common.h"
#include "../common/semaphore.h"
#include "game.h"

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
	char msg[TAILLE_BUFFER] = {'\0'};
	sprintf(msg, "1=%s", nom);

	if (envoyerMessage(sck, msg) == -1) {
		printf("erreur envois\n");
		return 1;
	} else
		printf("Message d'inscription envoye\n");

	// creer semaphore
	int semid = createSemaphore(SEM_KEY, IPC_CREAT);

	if (semid == -1) {
		printf("Impossible d'obtenir la semaphore\n");
		return 1;
	}

	// obtention de la memoire partagee
	int shmid = shmget(SHM_KEY, sizeof(struct game), 0444);

	if (shmid == -1) {
		printf("Impossible d'obtenir la memoire partagee\n");
		return 1;
	}

	struct game *g = (struct game *)shmat(shmid, NULL, 0);

	// boucle de lecture / envois de messages
	while (1) {
		int ret = 0;
		fd_set readfs;
		struct timeval tv;

		tv.tv_sec = 0;
		tv.tv_usec = 5;

		FD_ZERO(&readfs);

		FD_SET(sck, &readfs);

		ret = select(sck + 1, &readfs, NULL, NULL, &tv);

		if (ret > 0 && FD_ISSET(sck, &readfs)) {
			char message[TAILLE_BUFFER + 1] = {'\0'};

			int val = recevoirMessage(sck, message);
			if (val > 0)
				traiterMessage(sck, message, g, semid);
			else if (val == 0) {
				printf("Perte de connexion avec le serveur\n");
				break; // quitter la boucle
			}
		}
	}

	// detacher la memoire
	shmdt(g);

	// fermeture du semaphore
	deleteSemaphore(semid);

	// ferneture de la socket
	return fermerSocket(sck);
}
