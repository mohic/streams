/*****************************************************************\
*        Maxime LECAT (mlecat)                                    *
*        Christophe MOHIMONT (cmohimo)                            *
*                                                                 *
*        ce fichier contient les definitions, structures communes *
\*****************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <sys/ipc.h>
#include <sys/shm.h>

#define TAILLE_NOM 20
#define TAILLE_BUFFER 50
#define MAX_JOUEUR 4
#define SHM_KEY 10101
#define TOUR 20

typedef struct game
{
	int nbrJoueur;
	int score[MAX_JOUEUR];
	char nom[MAX_JOUEUR][TAILLE_NOM];
} game;

#endif

