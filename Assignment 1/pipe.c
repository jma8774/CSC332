#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 

int main() {
	// fd[0] is for reading, fd[1] is for writing
	int fd[2];
	pid_t pid, pid2;
	pipe(fd);
	pid = fork();	// create 1st child
	if(pid < 0) {
		printf("1st fork failed\n");
		return 1;
	} else if (pid == 0) {             
		printf("Hi, I am child 1 and I just got created!\n");
		dup2(fd[1], STDOUT_FILENO);     // copy of fd to write to
		close(fd[0]);            	// closing read because it is not used
		close(fd[1]);            	// closing write after use
		execlp("ls", "ls", "-F", NULL);
		exit(1);
	} else {                
		printf("Parent waiting for 1st child to finish... \n");
		wait(NULL);			// waiting for 2nd child to finish			
		close(fd[1]);
		printf("It is now back to parent\n");
	}

	pid2 = fork();            		// create 2nd child
		if(pid2 < 0) {
			printf("2nd fork failed\n");
		    return 1;
		} else if (pid2 == 0) {
		   	printf("Hi, I am child 2 and I just got created!\n");
		    dup2(fd[0], STDIN_FILENO);     	// create copy of fd of 1st child from read
		    close(fd[1]);           	// closing write because it is not used
		    close(fd[0]);             	// closing read after use
		    execlp("nl", "nl", NULL);  	// add nl to 1st child's ls -F
			exit(1);
		} else {
			printf("Parent waiting for 2nd child to finish... \n");
		    wait(NULL);			// waiting for 2nd child to finish		
			close(fd[1]);
			printf("It is now back to parent\n");
		}

	printf("Program is exiting...\n");
	return 0;

}