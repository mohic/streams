/****************************************************************************\
*        Maxime LECAT (mlecat)                                               *
*        Christophe MOHIMONT (cmohimo)                                       *
*                                                                            *
*        ce fichier contient la fonction main démarrant le programme serveur *
\****************************************************************************/

#include "serversck.h"
#include "game.h"
#include "../common/common.h"
#include "../common/semaphore.h"

#define SEM_KEY 12345

// remarque, n'accepte que 1 client pour le moment

int main (int argc, char* argv[])
{
	int i;

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

	// ouvrir la socket
	int sck = ouvrirSocket((unsigned short)port);

	if (sck == -1) {
		return 1;
	}

	printf("Serveur en écoute sur le port %d\n", port);

	// creation de la memoire partagee
	int shmid = shmget(SHM_KEY, sizeof(struct game), IPC_CREAT);
	struct game *g = (struct game *)shmat(shmid, NULL, 0);

	// creation du semaphore
	int semid = createSemaphore(SEM_KEY, IPC_CREAT | IPC_EXCL);

	// inscription d'un joueur
	fd_set readfs;
	struct timeval tv;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	int sockets[MAX_JOUEUR];
	int nombreJoueurActuel = 0;

	sockets[nombreJoueurActuel++] = accepterClient(sck);

	//TODO timer 30 secondes (voir avec signal)

	while(1) {
		int ret = 0;

		tv.tv_sec = 1;
		tv.tv_usec = 0;

		fd_set readfsJoueur;
		struct timeval tvJoueur;

		tvJoueur.tv_sec = 0;
		tvJoueur.tv_usec = 5;

		FD_ZERO(&readfsJoueur);

		for (i = 0; i < nombreJoueurActuel; i++)
			FD_SET(sockets[i], &readfsJoueur);

		ret = select(sockets[nombreJoueurActuel - 1] + 1, &readfsJoueur, NULL, NULL, &tvJoueur);

		if (ret > 0) {
			for (i = 0; i < nombreJoueurActuel; i++) {
				if (FD_ISSET(sockets[i], &readfsJoueur)) {;
					char message[TAILLE_BUFFER + 1] = {'\0'};
					if (recevoirMessage(sockets[i], message) > 0) {
						traiterMessage(sockets[i], message, g, i, semid);
					}
				}
			}
		}

		FD_ZERO(&readfs);
		FD_SET(sck, &readfs);
		
		if ((ret = select(sck + 1, &readfs, NULL, NULL, &tv)) < 0) {
			perror("select");
			return 1;
		} else if (ret == 0) {
			continue;
		} else {
			printf("Nouveau joueur présent\n");
			sockets[nombreJoueurActuel] = accepterClient(sck);
			nombreJoueurActuel++;
		}
	}

	// lecture / envois de messages
	//while(1) {
		//TODO boucle de jeu + gestion anti bloquant comme l'accept
	//}

	// fermer les sockets clients
	for (i = 0; i < nombreJoueurActuel; i++) {
		fermerSocket(sockets[i]);
	}

	// supprimer semaphore
	deleteSemaphore(semid);

	// detacher la memoire et la marquer comme detruite
	shmdt(g);
	shmctl(shmid, IPC_RMID, 0);

	// fermeture de la socket
	printf("Good bye !!!\n");
	return fermerSocket(sck);
}
