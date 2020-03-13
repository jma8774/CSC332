#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    // Executes argv[1] as Process_P1.c and argv[2] as Process.P2.c
    // First compile Process_P1.c, Process_P2.c, and Parent_Process.c
    // Then type "./parent ./p1 ./p2" to the command line
    // where parent, p1 and p2 are the names of the compiled C files\
    // I emailed you about how we should do this part but you didn't respond, so I did it this way

    pid_t pid1, pid2;

    pid1 = fork();
    if(pid1 < 0) {
        printf("Child 1 fork failed\n");
        return 1;
    } else if(pid1 == 0) { // child 1
        printf("This is child 1, who is about to execute Process_P1...\n\n");
        char *args[3] = {"ls", "-l", NULL};
        execvp("ls", args);
   return 0;
    } else { // parent
        printf("Parent waiting for child 1... \n");
        printf("PID of this child is %d\n", pid1);
        wait(NULL);

        printf("\nSleeping for 1 second...\n");
        sleep(1);

        pid2 = fork();
        if(pid2 < 0) {
            printf("Child 2 fork failed\n");
            return 1;
        } else if(pid2 == 0) { // child 2
            printf("This is child 2, who is about to execute Process_P2...\n\n");
            char *args[] = {argv[2],  NULL};
            execv(argv[2], args);
        } else { // parent
            printf("\nParent waiting for child 2... \n");
            printf("PID of this child is %d\n", pid2);
            wait(NULL);
        }
    }

    printf("\nParent exiting...\n");
    return 0;
}