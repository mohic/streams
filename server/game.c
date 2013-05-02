/***********************************************\
*        Maxime LECAT (mlecat)                  *
*        Christophe MOHIMONT (cmohimo)          *
*                                               *
*        ce fichier contient la logique du jeux *
\***********************************************/

#include "game.h"

int tuilePlacee[MAX_JOUEUR];

int traiterMessage(int sckClient, char *message, game *g, int joueur, int semid) {
	int i;
	if (strlen(message) <= 0)
		return -1;

	switch(message[0]) {
		case '1':
			down(semid);
				for (i = 0; i < strlen(message) - 2; i++)
					g->nom[joueur][i] = message[i+2];
				g->score[joueur] = -1;	
				g->nbrJoueur++;
				printf ("Nom du joueur: %s\n", g->nom[joueur]);
			up(semid);
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
