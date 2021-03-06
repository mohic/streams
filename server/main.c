/****************************************************************************\
*        Maxime LECAT (mlecat)                                               *
*        Christophe MOHIMONT (cmohimo)                                       *
*                                                                            *
*        ce fichier contient la fonction main démarrant le programme serveur *
\****************************************************************************/

#include <signal.h>
#include "serversck.h"
#include "game.h"
#include "../common/common.h"
#include "../common/semaphore.h"

#define TEMPS_ATTENTE 30

int gameStarted;
int timeElapsed;

void startgame(int sig)
{
	if (sig == SIGALRM) {
		printf("Le temps minimum de %d secondes est passe\n", TEMPS_ATTENTE);
		timeElapsed = 1;
	}
}

int main (int argc, char* argv[])
{
	int i;

	for (i = 1; i < argc; i++) {
		if (strncmp(argv[i], "-f", 2) == 0) {
			printf("Netoyage semaphore\n");
			int semid = createSemaphore(SEM_KEY, IPC_CREAT | 0666);
			deleteSemaphore(semid);
		}
	}

	// creation du semaphore
	int semid = createSemaphore(SEM_KEY, IPC_CREAT | IPC_EXCL | 0666);
	
	if (semid == -1) {
		printf("Une instance du serveur est deja demaree\n");
		return 0;
	}

	// init
	gameStarted = 0;
	timeElapsed = 0;

	// vérification # arguments
	if (argc <= 1 || argc >= 4) {
		printf("Usage: %s port [fichierErreur] [-f]\n", argv[0]);
		return 1;
	}

	// obtenir le port
	char *partieEnTrop;
	int port = strtol(argv[1], &partieEnTrop, 10);

	if (port < 1 || port > 65535 || partieEnTrop[0] != '\0') {
		printf("Le port doit être entre 1 et 65535\n");
		return 1;
	}

	// ouvrir le socket
	int sck = ouvrirSocket((unsigned short)port);

	if (sck == -1) {
		return 1;
	}

	printf("Serveur en écoute sur le port %d\n", port);

	// creation de la memoire partagee
	int shmid = shmget(SHM_KEY, sizeof(struct game), IPC_CREAT | 0666);
	struct game *g = (struct game *)shmat(shmid, NULL, 0);

	// initialisation de la memoire partagee
	down(semid);
		g->nbrJoueur = 0;
		
		for(i = 0; i < MAX_JOUEUR; i++)
			g->score[i] = -1;
			g->nom[i][0] = '\0'; // dit juste que la chaine est vide sans effacer le reste
	up(semid);

	// inscription d'un joueur
	fd_set readfs;
	struct timeval tv;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	int sockets[MAX_JOUEUR];

	for (i = 0; i < MAX_JOUEUR; i++)
		sockets[i] = -1;

	int nbrJoueurActuel = 0;

	sockets[nbrJoueurActuel++] = accepterClient(sck);

	printf("Appuyer sur CTRL+D pour quitter le serveur\n");

	// demarrer le timer
	signal(SIGALRM, startgame);
	alarm(TEMPS_ATTENTE);

	int quitter = 0;

	while(!quitter) {
		down(semid);
			int nbr = g->nbrJoueur;
		up(semid);
		
		if (!gameStarted && timeElapsed && g->nbrJoueur >= 2) {
			gameStarted = 1;
			demarrerPartie(sockets, nbrJoueurActuel, g, semid);
		}

		int ret = 0;

		tv.tv_sec = 0;
		tv.tv_usec = 5000;

		fd_set readfsJoueur;

		FD_ZERO(&readfsJoueur);

		int numSck;

		for (i = 0; i < MAX_JOUEUR; i++)
			if (sockets[i] != -1)
				FD_SET(numSck = sockets[i], &readfsJoueur);

		ret = select(numSck + 1, &readfsJoueur, NULL, NULL, &tv);

		if (ret > 0) {
			for (i = 0; i < MAX_JOUEUR; i++) {
				if (sockets[i] != -1 && FD_ISSET(sockets[i], &readfsJoueur)) {
					char message[TAILLE_BUFFER + 1] = {'\0'};
					int val = recevoirMessage(sockets[i], message);

					if (val > 0) {
						if (traiterMessage(sockets[i], message, g, i, semid, gameStarted) == -1)
							quitter = 1;
					} else if (val == 0) {
						down(semid);
							printf("Perte de connexion avec le joueur %s\n", g->nom[i]);
						up(semid);
						
						fermerSocket(sockets[i]);
						sockets[i] = -1;
						nbrJoueurActuel--;

						if (gameStarted && nbrJoueurActuel < 2) { // mettre fin a la partie
							int j;

							for (j = 0; j < MAX_JOUEUR; j++) {
								if (sockets[j] != -1) {
									printf("Annulation de la partie\n");
									envoyerMessage(sockets[j], "2");
									quitter = 1;
								}
							}
						} else {
							update(sockets, MAX_JOUEUR, g, semid, sockets[i], i);
						}
					}
				}
			}
		}

		if (quitter)
			break;

		FD_ZERO(&readfs);
		FD_SET(sck, &readfs);

		tv.tv_sec = 0;
		tv.tv_usec = 5000;
		
		if ((ret = select(sck + 1, &readfs, NULL, NULL, &tv)) < 0) {
			if (errno == EINTR) // erreur provoquee par le timer
				continue;

			perror("select");
			break;
		} else if (ret > 0) {
			printf("Nouveau joueur présent\n");
			int sckClient = accepterClient(sck);

			if (nbrJoueurActuel >= MAX_JOUEUR || gameStarted) {
				envoyerMessage(sckClient, "1=0"); // inscription refusee
				printf("inscription refusee\n");
				fermerSocket(sckClient); // fermer le socket
				continue;
			}

			for (i = 0; i < MAX_JOUEUR; i++) {
				if (sockets[i] == -1) {
					sockets[i] = sckClient;
					nbrJoueurActuel++;
					break;
				}
			}
		}

		tv.tv_sec = 0;
		tv.tv_usec = 5000;

		FD_ZERO(&readfs);
		FD_SET(STDIN_FILENO, &readfs);

		if ((ret = select(STDIN_FILENO + 1, &readfs, NULL, NULL, &tv)) < 0) {
			if (errno == EINTR) // erreur provoquee par le timer
				continue;

			perror("select");
			break;
		} else if (ret > 0) {
			char msg[3] = {'\0'};
			int size = read(STDIN_FILENO, msg, 2);

			if (size > 0) {
				while(read(STDIN_FILENO, NULL, 80) > 0);
			}

			if (size == 0) { // si ctrl-D alors quitter le server
				for (i = 0; i < MAX_JOUEUR; i++)
					if (sockets[i] != -1)
						envoyerMessage(sockets[i], "2"); // envoyer un partie annulee car le serveur s'arrete
				break;
			}
		}
	}

	// fermer les sockets clients
	for (i = 0; i < MAX_JOUEUR; i++) {
		if (sockets[i] != -1)
			fermerSocket(sockets[i]);
	}

	// supprimer semaphore
	deleteSemaphore(semid);

	// detacher la memoire et la marquer comme detruite
	shmdt(g);
	shmctl(shmid, IPC_RMID, 0);

	// fermeture de la socket
	printf("Good bye !!!\n");

	fermerSocket(sck);

	main(argc, argv); // reeinit le serveur

	return 0;
}
