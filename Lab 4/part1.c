#include <stdio.h>
#include <fcntl.h>      
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	pid_t child;
    size_t bufsize = 128;
    size_t malloc_size = 128;
	char *n = malloc(malloc_size);
    char *args[64];

    printf("Please enter the command > ");
    getline(&n, &bufsize, stdin);

	while(strcmp(n, "quit") !=0) {
        int i = 0;
        char* token = strtok(n, " \t\r\n\a");   // splitting the string by space and etc.
        while (token != NULL) {     // keep splitting and taking arguments until no more
            args[i] = malloc(malloc_size * sizeof(char)); // allocate memory for each argument
            args[i] = token;    // assign memory from args for token
            token = strtok(NULL, " \t\r\n\a");
            i ++;
        } 
        args[i] = malloc(malloc_size * sizeof(char)); // allocate memory at the end for NULL 
        args[i] = NULL;

        for(int j = 0; j <= i; j++) {
            printf("Argument[%d]: %s\n", j, args[j]);
        }
  
	    child = fork();
        if(child < 0) {
            printf("Debug: Fork failed\n");
        } else if(child == 0) {
            printf("\nChild executing command:\n");
			execvp(args[0], args);
            printf("Debug: Exec failed, exit(0)\n");
            exit(0);
		} else {
            wait(NULL);
            fflush(stdin);
            printf("######################################################################");
            printf("\n\nPlease enter the command > ");
            getline(&n, &bufsize, stdin);
        }
	}

	return 0;
}