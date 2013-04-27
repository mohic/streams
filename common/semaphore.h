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
