#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int n = 0;
int s = 0;

void *North()
{
    while(n < 10) {
        n++;
        printf("Northerner #%d can cross the bridge\n", n);
        printf("Northerner #%d traveling on the bridge\n\n", n);
        sleep(1);
        printf("Northerner #%d has left the bridge\n", n);
        printf("\n");
    }
    return NULL;
}

void *South()
{
    while(s < 10) {
        s++;
        printf("Southerner #%d can cross the bridge\n", s);
        printf("Southerner #%d traveling on the bridge\n\n", s);
        sleep(1);
        printf("Southerner #%d has left the bridge\n", s);
        printf("\n");
    }
    return NULL;
}

int main() {
	pthread_t tid[2]; // arr of threads
	
	pthread_create(&tid[0], NULL, &North, NULL); // creating north thread
	pthread_create(&tid[1], NULL, &South, NULL); // creating south thread

	pthread_join(tid[0], NULL); //kill pthread
	pthread_join(tid[1], NULL); //kill pthread
	return 0;
}