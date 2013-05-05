/***********************************************\
*        Maxime LECAT (mlecat)                  *
*        Christophe MOHIMONT (cmohimo)          *
*                                               *
*        ce fichier contient la logique du jeux *
\***********************************************/

#include "game.h"

int tuilePlacee[MAX_JOUEUR];

int traiterMessage(int sckClient, char *message, game *g, int joueur, int semid, int aDemarre) {
	int i;
	if (strlen(message) <= 0)
		return -1;

	switch(message[0]) {
		case '1':
			if (aDemarre) {
				printf("joueur refuse car partie deja commencee\n");
				envoyerMessage(sckClient, "1=0");
				//TODO fermer le socket client
				break;
			}

			down(semid);
				if (g->nbrJoueur >= MAX_JOUEUR) {
					printf("joueur refuse car maximum ateint\n");
					envoyerMessage(sckClient, "1=0"); // previent le client qu'il est refuse de jouer
					//TODO fermer le socket du client
					break;
				}

				for (i = 0; i < strlen(message) - 3; i++)
						g->nom[joueur][i] = message[i+2];
				g->nom[joueur][i] = '\0'; // eviter le '\n' final
				g->nbrJoueur++;
				printf ("Nom du joueur: %s\n", g->nom[joueur]);
			up(semid);

			envoyerMessage(sckClient, "1=1"); // previent le client qu'il est accepte
			printf("joueur accepte\n");

			break;
		
		case '3':
			down(semid);
				tuilePlacee[joueur] = 1;
				printf("Le joueur %s a placé sa tuile\n", g->nom[joueur]);
			up(semid);
			break;
		
		case '5':
			down(semid);
				message += 2;
				g->score[joueur] = atoi(message);
				printf("Score du joueur %s: %d\n", g->nom[joueur], g->score[joueur]); 
			up(semid);
			break;

		default:
			printf("Message inconnu: %s\n", message);
			return -1;
	}

	return 0;
}
