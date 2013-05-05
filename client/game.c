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

			envoyerMessage(sckClient, "5=1234");

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

void placerTuiles(int *cases, int tuile) {
		
	printf("Veuilliez placer la tuile: %d en encodant son emplacement", tuile);
	int placement;
	scanf("%d", &placement);
	cases[placement] = tuile;
	
}

int calculerScore(int *cases) {

	//suite placé à 1 car on a d'office une suite de 1 au premier
	int suite = 1;
	int score = 0;
	int tableauPoints[20] = {0, 1, 3, 5, 7, 9, 11, 15, 20, 25, 30, 35, 40, 50, 60, 70, 85, 100, 150, 300};
	
	int i;
	for(i = 0; i < 18; i++) {

		//si on tombe sur un joker
		if(cases[i+1] == 0) {
			suite++;
			cases[i+i] = cases[i];	//on met la tuile joker à la hauteur de la tuile précédente
						//pour éventuellement continuer la suite
		}
		//si la tuile est plus grande que la précédente
		else if(cases[i+1] >= cases[i]) {
			suite++;

		//si la tuile est plus petite que la précédente
		} else {
			score = score + tableauPoints[suite];
			suite = 1;
		}
	}

	return score;
}

