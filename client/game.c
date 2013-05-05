/***********************************************\
*        Maxime LECAT (mlecat)                  *
*        Christophe MOHIMONT (cmohimo)          *
*                                               *
*        ce fichier contient la logique du jeux *
\***********************************************/

#include "game.h"

void listerJoueurs(game *g, int semid)
{
	int i;

	printf("Liste des joueurs present:\n");
	
	down(semid);
		for (i = 0; i < g->nbrJoueur; i++)
			printf("Nom: %s, score: %d\n", g->nom[i], g->score[i]);
	up(semid);
}

int traiterMessage(int sckClient, char *message, game *g, int semid)
{
	int i;

	if (strlen(message) <= 0)
		return -1;

	switch(message[0]) {
		case '1':
			if (message[2] == '0')
				printf("Le serveur a refuse votre inscription\n");
			else
				printf("Vous etes maintenant inscrit pour la prochaine partie\n");

			listerJoueurs(g, semid);

			break;
		case '2':
			printf("La partie est annulee\n");
			return 2;
		case '3':
			message += 2;
			printf("La tuile est piochee: %s\n", message);
			//TODO positioner tuile
			envoyerMessage(sckClient, "3");
			break;
		case '4':
			printf("Fin de la partie\n");

			//TODO calculer score

			envoyerMessage(sckClient, "4=1234");

			break;
		/*case '5':
			down(semid);
				message += 2;
				g->score[joueur] = atoi(message);
				printf("Score du joueur %s: %d\n", g->nom[joueur], g->score[joueur]); 
			up(semid);
			break;*/

		default:
			printf("Message inconnu: %s\n", message);
			return -1;
	}

	return 0;
}
