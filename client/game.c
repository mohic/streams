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

	char *m = "Liste des joueurs present:\n";
	write(STDOUT_FILENO, m, strlen(m));
	
	down(semid);
		for (i = 0; i < g->nbrJoueur; i++) {
			char m2[6 + TAILLE_NOM + 1] = {'\0'};
			snprintf(m2, 6 + 6 + TAILLE_NOM + 1, "Nom: %s\n", g->nom[i]);
			write(STDOUT_FILENO, m2, strlen(m2));
		}
	up(semid);
}

void afficherTuiles()
{
	int i;

	char *m = "Emplacements : ";
	write(STDOUT_FILENO, m, strlen(m));

	for (i = 0; i < TOUR; i++) {
		char m2[6];
		snprintf(m2, 6, "[%.2d] ", i + 1);
		write(STDOUT_FILENO, m2, strlen(m2));
	}

	write(STDOUT_FILENO, "\n", 1);
	
	char *m3 = "Valeurs      : ";
	write(STDOUT_FILENO, m3, strlen(m));

	for(i = 0; i < TOUR; i++)
		if (tuiles[i] == 0) {
			char *m4 = "[  ] ";
			write(STDOUT_FILENO, m4, strlen(m4));
		} else if (tuiles[i] == 42) {
			char *m5 = "[ *] ";
			write(STDOUT_FILENO, m5, strlen(m5));
		} else {
			char m6[6];
			snprintf(m6, 6, "[%.2d] ", tuiles[i]);
			write(STDOUT_FILENO, m6, strlen(m6));
		}
	
	write(STDOUT_FILENO, "\n", 1);
}

void scoreJoueurs(game *g, int semid)
{
	int i;
	int top = -1;
	int numTop = -1;
	char *m = "Score des joueurs\n";
	write(STDOUT_FILENO, m, strlen(m));

	down(semid);
		for(i = 0; i < g->nbrJoueur; i++) {
			char m2[TAILLE_NOM + 11 + 10 + 1] = {'\0'};
			snprintf(m2, TAILLE_NOM + 11 + 10 + 1, "%s a obtenu %d\n", g->nom[i], g->score[i]);
			write(STDOUT_FILENO, m2, strlen(m2));
			if(g->score[i] > top) {
				numTop = i;
				top = g->score[i];
			}
		}
	char gagnant[TAILLE_NOM + 16 +1] = {'\0'};
	snprintf(gagnant, TAILLE_NOM + 16 + 1, "Le gagnant est %s\n", g->nom[numTop]);
	write(STDOUT_FILENO, gagnant, strlen(gagnant));
	up(semid);
}

int traiterMessage(int sckClient, char *message, game *g, int semid)
{
	int i;

	if (strlen(message) <= 0)
		return -1;

	switch(message[0]) {
		case '1':
			if (message[2] == '0') {
				char *m = "Le serveur a refuse votre inscription\n";
				write(STDOUT_FILENO, m, strlen(m));
			} else {
				char *m2 = "Vous etes maintenant inscrit pour la prochaine partie\n";
				write(STDOUT_FILENO, m2, strlen(m2));
				listerJoueurs(g, semid);

			}

			break;
		case '2':
			i = 1; //inutile mais on ne peux pas mettre de declaration de variable comme premiere instruction de case
			char *m3 = "La partie est annulee. Il semblerait que comme vous etes le dernier joueur, vous avez gagne\n";
			write(STDOUT_FILENO, m3, strlen(m3));
			return 2;
		case '3':
			message += 2;
			placerTuiles(atoi(message));
			envoyerMessage(sckClient, "3");
			break;
		case '4':
			i = 1;
			char *m4 = "Fin de la partie\n";
			write(STDOUT_FILENO, m4, strlen(m4));

			int score = calculerScore();
			char msg[TAILLE_BUFFER] = {'\0'};
			sprintf(msg, "5=%d", score);
			envoyerMessage(sckClient, msg);
			break;	
		case '5':
			scoreJoueurs(g, semid);
			return 2;
		default:
			i = 1;
			char m5[17 + strlen(message) + 1];
			snprintf(m5, sizeof(m5), "Message inconnu: %s\n", message);
			write(STDOUT_FILENO, m5, strlen(m5));
			return -1;
	}

	return 0;
}

void placerTuiles(int tuile) {
	int placement;

	while(1) {
		if (tuile == 42) {
			char *m1 = "Tuile recue: Joker\n";
			write(STDOUT_FILENO, m1, strlen(m1));
		} else {
			char m[14 + 10];
			snprintf(m, sizeof(m), "Tuile recue: %d\n", tuile);
			write(STDOUT_FILENO, m, strlen(m));
		}

		afficherTuiles();
		char *msg = "Veuillez encoder son emplacement: ";
		write(STDOUT_FILENO, msg, strlen(msg));
		char buffer[3] = {'\0'};
		read(STDOUT_FILENO, buffer, 2);
		//TODO vider le trop plein

		write(STDOUT_FILENO, "\n", 1);
		
		placement = atoi(buffer);

		if (placement < 1 || placement > TOUR) {
			char m2[40];
			snprintf(m2, 40, "Veuillez entrez un nombre entre 1 et %d\n", TOUR);
			write(STDOUT_FILENO, m2, strlen(m2));
		} else if (tuiles[placement - 1] != 0) {
			char *m3 = "Emplacement deja prit. Choisissez en un autre\n";
			write(STDOUT_FILENO, m3, strlen(m3));
		} else {
			char *m4 = "Tuile placee. En attente du placement des autres joueurs\n";
			write(STDOUT_FILENO, m4, strlen(m4));
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

	score = score + tableauPoints[suite];

	return score;
}

