#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();
    if(pid < 0) {
        printf("1st fork failed\n");
		return 1;
    } else if(pid == 0) {
        printf("Hi I am child 1!\n");
        printf("Child 1 exiting...\n");
        exit(0);
    } else {
        printf("Parent waiting...\n");
        wait(NULL);
        printf("Parent done waiting for child 1\n");
    }

    pid = fork();
    if(pid < 0) {
        printf("2nd fork failed\n");
		return 1;
    } else if(pid == 0) {
        printf("Hi I am child 2!\n");
        printf("Child 2 exiting...\n");
        exit(0);
    } else {
        printf("Parent waiting...\n");
        wait(NULL);
        printf("Parent done waiting for child 2\n");
    }

    printf("Parent exiting...\n");
    return 0;
}