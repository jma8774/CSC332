#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<time.h> 

pthread_mutex_t lock, agent_lock, match_lock, paper_lock, tobacco_lock;
int paperCnt = 0, tobaccoCnt = 0, matchCnt = 0;
int rounds = 10;

void *agent_func();
void *tobacco_func();
void *paper_func();
void *match_func();

int main() {
    pthread_t agent, tobacco, match, paper;
    int err;
    srand(time(0));
    if (    pthread_mutex_init(&lock, NULL) != 0 && 
            pthread_mutex_init(&agent_lock, NULL) != 0 &&
            pthread_mutex_init(&match_lock, NULL) != 0 &&
            pthread_mutex_init(&paper_lock, NULL) != 0 &&
            pthread_mutex_init(&tobacco_lock, NULL) != 0){
        printf("\n mutex init failed\n");
        return 1;
    }

    pthread_mutex_lock(&agent_lock);
    pthread_mutex_lock(&match_lock);
    pthread_mutex_lock(&paper_lock);
    pthread_mutex_lock(&tobacco_lock);
    pthread_mutex_unlock(&lock);

    // Agent thread
    err = pthread_create(&agent, NULL, &agent_func, NULL);
    if (err != 0) {
        printf("\ncan't create thread :[%s]", strerror(err));
        return 1;
    }

    // tobacco thread
    err = pthread_create(&tobacco, NULL, &tobacco_func, NULL);
    if (err != 0) {
        printf("\ncan't create thread :[%s]", strerror(err));
        return 1;
    }

    // Paper thread
    err = pthread_create(&paper, NULL, &paper_func, NULL);
    if (err != 0) {
        printf("\ncan't create thread :[%s]", strerror(err));
        return 1;
    }
    
    // Match thread
    err = pthread_create(&match, NULL, &match_func, NULL);
    if (err != 0) {
        printf("\ncan't create thread :[%s]", strerror(err));
        return 1;
    }

    pthread_join(agent, NULL);
    pthread_cancel(tobacco);
    pthread_cancel(match);
    pthread_cancel(paper);

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&agent_lock);
    pthread_mutex_destroy(&tobacco_lock);
    pthread_mutex_destroy(&paper_lock);
    pthread_mutex_destroy(&match_lock);
    printf("There are %d tobacco, %d paper and %d match left on the table\n", tobaccoCnt, paperCnt, matchCnt);
    return 0;
}

void* agent_func() {
    while(rounds > 0) {
        pthread_mutex_lock(&lock);
        printf("\n\nRounds left %d:\n", rounds);
        int randNum = (rand() % 3) + 1; // Pick a random number from 1-3
        if (randNum == 1) {
            tobaccoCnt ++;
            paperCnt ++;
            printf("Agent handed out tobacco and paper...\n");
            pthread_mutex_unlock(&match_lock); // Wake up smoker with match
        } else if (randNum == 2) {
            tobaccoCnt ++;
            matchCnt ++;
            printf("Agent handed out tobacco and match...\n");
            pthread_mutex_unlock(&paper_lock); // Wake up smoker with paper
        } else {
            matchCnt ++;
            paperCnt ++;
            printf("Agent handed out match and paper...\n");
            pthread_mutex_unlock(&tobacco_lock); // Wake up smoker with tobacco
        }
        pthread_mutex_unlock(&lock);
        pthread_mutex_lock(&agent_lock); // Agent sleeps
    }
    return NULL;
}

void *tobacco_func() {
    while(rounds > 0) {
        pthread_mutex_lock(&tobacco_lock); // Put to sleep immediately, waiting for unlock
        pthread_mutex_lock(&lock); // Wait for CS to be available
        paperCnt --;
        matchCnt --;
        printf("Smoker(tobacco) took them!\n");
        printf("Smoker(tobacco) is smoking!\n");
        rounds --;
        sleep(2);
        pthread_mutex_unlock(&agent_lock); // Wake Agent up
        pthread_mutex_unlock(&lock); // Release CS
    }
    return NULL;
}

void *paper_func() {
    while(rounds > 0) {
        pthread_mutex_lock(&paper_lock); // Put to sleep immediately, waiting for unlock
        pthread_mutex_lock(&lock); // Wait for CS to be available
        tobaccoCnt --;
        matchCnt --;
        printf("Smoker(paper) took them!\n");
        printf("Smoker(paper) is smoking!\n");
        rounds --;
        sleep(2);
        pthread_mutex_unlock(&agent_lock); // Wake Agent up
        pthread_mutex_unlock(&lock); // Release CS
    }
    return NULL;
}

void *match_func() {
    while(rounds > 0) {
        pthread_mutex_lock(&match_lock); // Put to sleep immediately, waiting for unlock
        pthread_mutex_lock(&lock); // Wait for CS to be available
        tobaccoCnt --;
        paperCnt --;
        printf("Smoker(match) took them!\n");
        printf("Smoker(match) is smoking!\n");
        rounds --;
        sleep(2);
        pthread_mutex_unlock(&agent_lock); // Wake Agent up
        pthread_mutex_unlock(&lock); // Release CS
    }
    return NULL;
}