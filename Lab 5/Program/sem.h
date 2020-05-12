#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

// Union structure used to call semctl()
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

// Create semaphore with sem_id and sem_val
int SEM_SET(int sem_id, int sem_val) {
	union semun sem_union;
	sem_union.val = sem_val;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
        return -1;
	return 0;
}

// Delete semaphores
void SEM_DEL(int sem_id) {
	union semun sem_union;
	if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        fprintf(stderr, "Failed to delete semaphore\n");
}

// Performs semaphore P operation, called when trying to enter CS
int SEM_P(char person[12], int sem_id) {
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1; // P operation -1
	sem_b.sem_flg = SEM_UNDO;
    printf("%s is waiting\n", person);
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "P() failed\n");
		return -1;
	}
    printf("\n%s is done waiting\n", person);
	return 0;
}

// Performs semaphore V operation, called upon exit from CS
int SEM_V(int sem_id) {
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1; // V operation +1
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "V() failed\n");
		return -1;
	}
	return 0;
}