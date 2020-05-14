#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int n = 0;
int s = 0;
pthread_mutex_t north, south, lock;

void *North()
{
    while(n < 10) {
        pthread_mutex_lock(&lock); // lock is initially unlocked, northerners will go first
        n++;
        printf("Northerner #%d can cross the bridge\n", n);
        printf("Northerner #%d traveling on the bridge\n", n);
        sleep(1);
        printf("Northerner #%d has left the bridge\n\n", n);
        pthread_mutex_unlock(&lock); // once the notherner crosses, we unlock the CS (bridge), meaning it is available for use now
        pthread_mutex_unlock(&south); // alert southerner that it is ready to be crossed
        pthread_mutex_lock(&north); // put northerners to sleep until it is unlocked again (initialized as locked)
    }
        return NULL;
}

void *South()
{
    while(s < 10) {
        pthread_mutex_lock(&south); // once south is woken up by north, we lock south
        pthread_mutex_lock(&lock); // south will lock the CS (bridge) as well for crossing
        s++;
        printf("Southerner #%d can cross the bridge\n", s);
        printf("Southerner #%d traveling on the bridge\n", s);
        sleep(1);
        printf("Southerner #%d has left the bridge\n\n", s);
        pthread_mutex_unlock(&north); // after finishing, we alert the north that the bridge is available
        pthread_mutex_unlock(&lock); // unlock the CS
    }
    return NULL;
}

int main() {
    // error checking when creating mutex
    if (pthread_mutex_init(&north, NULL) != 0 || pthread_mutex_init(&south, NULL) != 0 || pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }
    pthread_mutex_lock(&south); // initialize north mutex as locked
    pthread_mutex_lock(&north); // initialize south mutex as locked
    pthread_mutex_unlock(&lock); // initialize lock as unlocked

	pthread_t tid[2]; // arr of threads

	pthread_create(&tid[0], NULL, &North, NULL); // creating north thread
	pthread_create(&tid[1], NULL, &South, NULL); // creating south thread

	pthread_join(tid[0], NULL); //kill pthread
	pthread_join(tid[1], NULL); //kill pthread

    pthread_mutex_destroy(&north); // kill mutex
    pthread_mutex_destroy(&south); // kill mutex
    pthread_mutex_destroy(&lock); // kill mutex
	return 0;
}