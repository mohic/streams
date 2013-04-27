#ifndef __COMMON_H__
#define __COMMON_H__

#include <sys/ipc.h>
#include <sys/shm.h>

#define TAILLE_NOM 20
#define MAX_JOUEUR 4
#define SHM_KEY 10101

typedef struct game
{
	int nbrJoueur;
	int score[MAX_JOUEUR];
	char nom[MAX_JOUEUR][TAILLE_NOM];
} game;

#endif

