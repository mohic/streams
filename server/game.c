#include "game.h"

int traiterMessage(int sckClient, char *message, game *g, int joueur, int semid) {
	int i;
	if (strlen(message) <= 0)
		return -1;

	switch(message[0]) {
		case '1':
			down(semid);

			for (i = 0; i < strlen(message) - 2; i++)
				g->nom[joueur][i] = message[i+2];
				
			g->nbrJoueur++;
			up(semid);
			
			printf ("Nom du joueur: %s\n", g->nom[joueur]);
			break;
		default:
			printf("Message inconnu: %s\n", message);
	}

	return 0;
}
