/*
Hi Professor, I made my code so that the attempt.txt file will keep track T(P) of each process.
Line 1 in the file represents N_ATT
Line 2 in the file represents T(P) of Dad
Line 3 in the file represents T(P) of Son 1
Line 4 in the file represents T(P) of Son 2

I also have a file called interest.txt which keeps track of whether process Dad, Son 1 or Son 2 has expressed interest in accessing the CS
Line 1 is the interest of the Dad: 0 = False, 1 = True
Line 1 is the interest of Son 1: 0 = False, 1 = True
Line 1 is the interest of Son 2: 0 = False, 1 = True

- Jia Ming Ma
*/

#include <stdio.h>
#include <stdlib.h>
#include "sem.h" // Including my sem.h file into this c file

#define CHILD      			0  			/* Return value of child proc from fork call */
#define TRUE       			0  
#define FALSE      			1

FILE *fp, *fp1, *fp2, *fp3, *fp4;			/* File Pointers */

// My Variables
int sem;
int dad_interest, son1_interest, son2_interest; 	// Integer describing whether the process is interested in joining the CS 0 = False, 1 = True
int dad_counter, son1_counter, son2_counter;		// Counter of the number of processes that went after it has expressed interest

void printAttempt() {
	int N_Att;
	fp = fopen("attempt", "r+");
	fscanf(fp, "%d %d %d %d", &N_Att, &dad_counter, &son1_counter, &son2_counter);
	printf("Attempts file: %d %d %d %d\n", N_Att, dad_counter, son1_counter, son2_counter);
	fclose(fp);
}

void setDadInterest(int b) {
	fp2 = fopen("interest", "r+");
	fscanf(fp2, "%d %d %d", &dad_interest, &son1_interest, &son2_interest);
	fseek(fp2, 0, SEEK_SET);
	fprintf(fp2, "%d\n%d\n%d\n", b, son1_interest, son2_interest);
	fclose(fp2);
}

void setSon1Interest(int b) {
	fp2 = fopen("interest", "r+");
	fscanf(fp2, "%d %d %d", &dad_interest, &son1_interest, &son2_interest);
	fseek(fp2, 0, SEEK_SET);
	fprintf(fp2, "%d\n%d\n%d\n", dad_interest, b, son2_interest);
	fclose(fp2);
}

void setSon2Interest(int b) {
	fp2 = fopen("interest", "r+");
	fscanf(fp2, "%d %d %d", &dad_interest, &son1_interest, &son2_interest);
	fseek(fp2, 0, SEEK_SET);
	fprintf(fp2, "%d\n%d\n%d\n", dad_interest, son1_interest, b);
	fclose(fp2);
}

main()
{
	int pid;						// Process ID after fork call
	int i;							// Loop index
	int N;							// Number of times dad does update
	int N_Att;						// Number of time sons allowed to do update
	int status;						// Exit status of child process
	int bal1, bal2;					// Balance read by processes
	int flag, flag1;				// End of loop variables
	
	// Initializing semaphore
	if((sem = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666)) == -1) {
		printf("Error making sem");
		exit(1);
	}
	SEM_SET(sem, 1);

	// Initialization of process interests
	
	fp = fopen("interest", "w");
	fprintf(fp, "0\n0\n0\n");
	fclose(fp);

	//Initialize the file balance to be $100
	fp1 = fopen("balance","w");
	bal1 = 200;
	fprintf(fp1, "%d\n", bal1);
	fclose(fp1);
	
	//Initialize the number of attempts to be 20
	fp4 = fopen("attempt", "w");
	N_Att = 20;
	fprintf(fp4, "%d\n0\n0\n0\n", N_Att);
	fclose(fp4);
	
	//Create child processes that will do the updates
		if ((pid = fork()) == -1) 
	{
		//fork failed!
		perror("fork");
		exit(1);
	}
	
	if (pid == CHILD){
	//First Child Process. Dear old dad tries to do some updates.
		char person[12] = "Dad";
		N=5;
		for(i=1;i<=N; i++)
		{
			// This portion of code updates the interest of Dad to 1
			setDadInterest(1);

			SEM_P(person, sem);

			// This portion of code gets the most updated interest of each process
			fp2 = fopen("interest", "r+");
			fscanf(fp2, "%d %d %d", &dad_interest, &son1_interest, &son2_interest);
			fclose(fp2);
			
			// This portion of the code checks if son1 and son2 are interested and is waiting, if so, we increment their T(P) counter
			fp = fopen("attempt", "r+");
			fscanf(fp, "%d %d %d %d", &N_Att, &dad_counter, &son1_counter, &son2_counter);
			if(son1_interest == 1)
				son1_counter ++;
			if(son2_interest == 1)
				son2_counter ++;
			fseek(fp, 0, SEEK_SET);
			fprintf(fp, "%d\n%d\n%d\n%d\n", N_Att, dad_counter, son1_counter, son2_counter);
			fclose(fp);
			
			// This portion is Professor's code
			printf("Dear old dad is trying to do update.\n");
			fp1 = fopen("balance", "r+");
			fscanf(fp1, "%d", &bal2);
			printf("Dear old dad reads balance = %d \n", bal2);
			
			//Dad has to think (0-14 sec) if his son is really worth it
			sleep(rand()%15);
			fseek(fp1, 0, SEEK_SET);
			bal2 += 60;
			printf("Dear old dad writes new balance = %d \n", bal2);
			fprintf(fp1, "%d \n", bal2);
			fclose(fp1);

			printf("Dear old dad is done doing update. \n");
			sleep(rand()%5);	/* Go have coffee for 0-4 sec. */
			// End of Professor's code

			printAttempt();
			// This portion of the code updates Dad's interest to 0 before exiting CS
			setDadInterest(0);
			SEM_V(sem);
		}
	}
	else
	{
		//Parent Process. Fork off another child process.
		if ((pid = fork()) == -1)
		{
			//Fork failed!
			perror("fork");
			exit(1);
		}
		if (pid == CHILD)
		{
			printf("First Son's Pid: %d\n",getpid());
			char person[12] = "First son";
			//Second child process. First poor son tries to do updates.
			flag = FALSE;
			while(flag == FALSE) 
			{
				// Son 1 wants to enter CS, Setting interest to 1
				setSon1Interest(1);
				// Wait to see if CS is available for use
				SEM_P(person, sem);
				
				// This portion of code gets the most updated interest of each process
				fp2 = fopen("interest", "r+");
				fscanf(fp2, "%d %d %d", &dad_interest, &son1_interest, &son2_interest);
				fclose(fp2);

				// This portion of the code checks if dad and son2 are interested and is waiting, if so, we increment their T(P) counter
				fp = fopen("attempt", "r+");
				fscanf(fp, "%d %d %d %d", &N_Att, &dad_counter, &son1_counter, &son2_counter);
				if(dad_interest == 1)
					dad_counter ++;
				if(son2_interest == 1)
					son2_counter ++;
				fseek(fp, 0, SEEK_SET);
				fprintf(fp, "%d\n%d\n%d\n%d\n", N_Att, dad_counter, son1_counter, son2_counter); // made minor changes to this
				fclose(fp);

				// This portion is Professor's code
				fp3 = fopen("attempt" , "r+");
				fscanf(fp3, "%d", &N_Att);
				if(N_Att == 0)
				{
					fclose(fp3);
					flag = TRUE;
				}
				else
				{
					printf("Poor SON_1 wants to withdraw money.\n");
					fp2 = fopen("balance", "r+");
					fscanf(fp2,"%d", &bal2);
					printf("Poor SON_1 reads balance. Available Balance: %d \n", bal2);
					if (bal2 == 0)
					{
						fclose(fp2);
						fclose(fp3);
					}
					else
					{
						sleep(rand()%5);
						fseek(fp2,0L, 0);
						bal2 -=20;
						printf("Poor SON_1 write new balance: %d \n", bal2);
						fprintf(fp2,"%d\n", bal2);
						fclose(fp2);
						printf("poor SON_1 done doing update.\n");
						fseek(fp3, 0, SEEK_SET);
						N_Att -=1;
						fprintf(fp3, "%d\n%d\n%d\n%d\n", N_Att, dad_counter, son1_counter, son2_counter); // made minor changes to this
						fclose(fp3);
					}
				}
				// End of Professor's code

				printAttempt();
				// Son 1 wants to leave CS, Setting interest to 0
				setSon1Interest(0);
				// Son 1 is leaving CS
				SEM_V(sem);
			}
		}
		else
		{
		//Parent Process. Fork off one more child process.
			if ((pid = fork()) == -1) 
			{
				//fork failed!
				perror("fork");
				exit(1);
			}
			if (pid == CHILD)
			{
				char person[12] = "Second Son";
				printf("Second Son's Pid: %d\n",getpid());
				//Third child process. Second poor son tries to do updates.
				flag1 = FALSE;
				while(flag1 == FALSE) 
				{
					// Son 2 wants to enter CS, Setting interest to 1
					setSon2Interest(1);
					// Wait to see if CS is available for use
					SEM_P(person, sem);

					// This portion of code gets the most updated interest of each process
					fp2 = fopen("interest", "r+");
					fscanf(fp2, "%d %d %d", &dad_interest, &son1_interest, &son2_interest);
					fclose(fp2);

					// This portion of the code checks if dad and son1 are interested and is waiting, if so, we increment their T(P) counter
					fp = fopen("attempt", "r+");
					fscanf(fp, "%d %d %d %d", &N_Att, &dad_counter, &son1_counter, &son2_counter);
					if(dad_interest == 1)
						dad_counter ++;
					if(son1_interest == 1)
						son1_counter ++;
					fseek(fp, 0, SEEK_SET);
					fprintf(fp, "%d\n%d\n%d\n%d\n", N_Att, dad_counter, son1_counter, son2_counter);
					fclose(fp);

					// This portion is Professor's code
					fp3 = fopen("attempt" , "r+");
					fscanf(fp3, "%d", &N_Att);
					if(N_Att == 0)
					{
						fclose(fp3);
						flag1 = TRUE;
					}
					else
					{
						printf("Poor SON_2 wants to withdraw money.\n");
						fp2 = fopen("balance", "r+");
						fscanf(fp2,"%d", &bal2);
						printf("Poor SON_2 reads balance. Available Balance: %d \n", bal2);
						if (bal2 == 0)
						{
							fclose(fp2);
							fclose(fp3);
						}
						else
						{
							sleep(rand()%5);
							fseek(fp2,0L, 0);
							bal2 -=20;
							printf("Poor SON_2 write new balance: %d \n", bal2);
							fprintf(fp2,"%d\n", bal2);
							fclose(fp2);

							printf("poor SON_2 done doing update.\n");
							fseek(fp3, 0, SEEK_SET);
							N_Att -=1;
							fprintf(fp3, "%d\n%d\n%d\n%d\n", N_Att, dad_counter, son1_counter, son2_counter); // made minor changes to this
							fclose(fp3);
						}
					}
					// End of Professor's Code

					printAttempt();
					// Son 2 wants to leave CS, Setting interest to 0
					setSon2Interest(0);
					// Son 2 is leaving CS
					SEM_V(sem);
				}
			}
			else
			{
				//Now parent process waits for the child processes to finish
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);

				// This portion of code gets the most updated interest of each process and prints it
				fp = fopen("attempt", "r+");
				fscanf(fp, "%d %d %d %d", &N_Att, &dad_counter, &son1_counter, &son2_counter);
				fclose(fp);

				printf("\nPrinting wait time in terms of number of processes that went after P expressed interest in CS\n");
				printf("Dad wait time T(Dad): %d\n", dad_counter);
				printf("Son 1 wait time T(Son 1): %d\n", son1_counter);
				printf("Son 2 wait time T(Son 2): %d\n", son2_counter);
				SEM_DEL(sem);
			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
}




