/***********************************************\
*        Maxime LECAT (mlecat)                  *
*        Christophe MOHIMONT (cmohimo)          *
*                                               *
*        ce fichier contient la logique du jeux *
\***********************************************/

#include "game.h"

int tuiles[40];
int nbrRestant;
int tuilePlacee[MAX_JOUEUR];

//int scks[];
//int tailleScks;

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
			tuilePlacee[joueur] = 1;

			down(semid);
				printf("Le joueur %s a placé sa tuile\n", g->nom[joueur]);

			int ontTousPlace = 1;

			for (i = 0; i < g->nbrJoueur; i++)
				if (!tuilePlacee[i])
					ontTousPlace = 0;
			up(semid);

			//TODO piocher le suivant
			//if (ontTousPlace)
			//	piocherTuile(scks, tailleScks);

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

void piocherTuile(int sockets[], int taille)
{
	int i;

	int posTuile = rand() % nbrRestant;

	char msg[5] = {'\0'};
	sprintf(msg, "3=%d", tuiles[posTuile]);

	printf("Tuile piochee: %d\n", tuiles[posTuile]);

	tuiles[posTuile] = tuiles[nbrRestant - 1];
	nbrRestant--;

	for(i = 0; i < taille; i++)
		envoyerMessage(sockets[i], msg);
}

void demarrerPartie(int sockets[], int taille)
{
	int i;

	//scks = sockets;
	//tailleScks = taille;

	printf("La partie commence\n");

	// initialise le generateur de nombre pseudo-aleatoire
	srand(time(NULL));

	for (i = 0; i < 10; i++) // creer les tuiles de 1 a 10
		tuiles[i] = i + 1;

	for (i = 0; i < 18; i += 2) { // creer les tuiles de 11 a 19
		tuiles[i + 10] = i + 11;
		tuiles[i + 11] = i + 11;
	}

	for (i = 0; i < 9; i++) // creer les tuiles de 20 a 30
		tuiles[i + 29] = i + 20;

	tuiles[39] = 0; // creer le joker (represente sous la valeur de 0 en memoire)

	nbrRestant = 40;

	// demarrer le jeu en piochant la 1ere tuile
	piocherTuile(sockets, taille);
}
