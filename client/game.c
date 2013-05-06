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
			printf("Nom: %s\n", g->nom[i]);
	up(semid);
}

void afficherTuiles()
{
	int i;

	printf("Emplacements : ");
	for (i = 0; i < 20; i++)
		printf("[%.2d] ", i + 1);
	
	printf("\nValeurs      : ");
	for(i = 0; i < 20; i++)
		if (tuiles[i] == 0)
			printf("[  ] ");
		else if (tuiles[i] == 42)
			printf("[ *] ");
		else
			printf("[%.2d] ", tuiles[i]);
	
	printf("\n");
}

void scoreJoueurs(game *g, int semid)
{
	int i;

	printf("Score des joueurs\n");

	down(semid);
		for(i = 0; i < g->nbrJoueur; i++)
			printf("%s a obtenu %d\n", g->nom[i], g->score[i]);
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
			placerTuiles(atoi(message));
			envoyerMessage(sckClient, "3");
			break;
		case '4':
			printf("Fin de la partie\n");

			int score = calculerScore();
			char msg[TAILLE_BUFFER] = {'\0'};
			sprintf(msg, "5=%d", score);
			envoyerMessage(sckClient, msg);
			break;	
		case '5':
			scoreJoueurs(g, semid);
			break;
		default:
			printf("Message inconnu: %s\n", message);
			return -1;
	}

	return 0;
}

void placerTuiles(int tuile) {
	int placement;
	while(1) {
		printf("Tuile recue: %d\n", tuile);
		afficherTuiles();
		char *msg = "Veuillez encoder son emplacement: ";
		write(STDOUT_FILENO, msg, strlen(msg));
		char buffer[3] = {'\0'};
		read(STDOUT_FILENO, buffer, 2);
		//TODO vider le trop plein

		write(STDOUT_FILENO, "\n", 1);
		
		placement = atoi(buffer);

		if (placement < 1 || placement > 20)
			printf("Veuillez entrez un nombre entre 1 et 20\n");
		else if (tuiles[placement - 1] != 0)
			printf("Emplacement deja prit. Choisissez en un autre\n");
		else {
			printf("Tuile placee. En attente du placement des autres joueurs\n");
			break;
		}
	}

	tuiles[placement - 1] = tuile;
}

int calculerScore() {

	int suite = 0;
	int score = 0;
	int tableauPoints[20] = {0, 1, 3, 5, 7, 9, 11, 15, 20, 25, 30, 35, 40, 50, 60, 70, 85, 100, 150, 300};
	
	int i;
	for(i = 0; i <= 18; i++) {

		//si on tombe sur un joker
		if(tuiles[i+1] == 42) {
			suite++;
			tuiles[i+1] = tuiles[i];	//on met la tuile joker à la hauteur de la tuile précédente
						//pour éventuellement continuer la suite
		}
		//si la tuile est plus grande que la précédente
		else if(tuiles[i+1] >= tuiles[i]) {
			suite++;

		//si la tuile est plus petite que la précédente
		} else {
			score = score + tableauPoints[suite];
			suite = 0;
		}
	}

	return score;
}

