/********************************************************************\
*        Maxime LECAT (mlecat)                                       *
*        Christophe MOHIMONT (cmohimo)                               *
*                                                                    *
*        ce fichier contient la definition des fonctions de creation *
*        de semaphores                                               *
\********************************************************************/

#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int createSemaphore(key_t key, int flags);
int deleteSemaphore(int semid);
int down(int semid);
int up(int semid);

#endif
