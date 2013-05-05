/**********************************************************************\
*        Maxime LECAT (mlecat)                                         *
*        Christophe MOHIMONT (cmohimo)                                 *
*                                                                      *
*        ce fichier contient la definition des fonctions de la logique *
*        du jeux                                                       *
\**********************************************************************/

#ifndef __game_h__
#define __game_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/common.h"
#include "../common/semaphore.h"

void listerJoueurs(game *g, int semid);
int traiterMessage(int sckClient, char *message, game *g, int semid);
int calculerScore(int *cases);

#endif
