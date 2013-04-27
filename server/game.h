#ifndef __game_h__
#define __game_h__

#include <stdio.h>
#include <string.h>
#include "../common/common.h"
#include "../common/semaphore.h"

int traiterMessage(int sckClient, char *message, game *g, int joueur, int semid);

#endif
