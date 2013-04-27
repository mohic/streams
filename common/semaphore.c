#include "semaphore.h"

int createSemaphore(key_t key, int flags)
{
	int semid = semget(key, 1, flags);

	semctl(semid, 0, SETVAL, 1);

	return semid;
}

int deleteSemaphore(int semid)
{
	return semctl(semid, 0, IPC_RMID, 0);
}

int down(int semid)
{
	struct sembuf op;

	op.sem_num = 0;
	op.sem_op  = -1;
	op.sem_flg = 0;

	return semop(semid, &op, 1);
}

int up(int semid)
{
	struct sembuf op;

	op.sem_num = 0;
	op.sem_op  = 1;
	op.sem_flg = 0;

	return semop(semid, &op, 1);
}
