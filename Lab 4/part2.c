#include <stdio.h>
#include <fcntl.h>      
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Asking for user inputs based on the txt file (# of students, # of chapters, # of homework per chapter)
    int students = 0;
    int chapters = 0;
    int homework = 0;
    printf("How many students are there > "); // in our case, we would input 10 because 10 students/rows
    scanf("%d", &students);
    printf("How many chapters are there > "); // in our case, we would input 2 for y = 2
    scanf("%d", &chapters);
    printf("How many homework are there for each chapter > "); // in our case, we would input 2 for x = 2
    scanf("%d", &homework);
    printf("\n[Log] Students: %d Chapters: %d Homework: %d\n", students, chapters, homework);
    
    // Parsing grades from our txt file
    FILE* file = fopen("quiz_grades.txt","r"); 
    if (file==NULL) { 
        printf("[Log] No such file\n"); 
        return 0; 
    }
    close(file);
    
    int grades[students][chapters * homework];
    for(int i = 0; i < students; i ++) {
        for(int j = 0; j < chapters*homework; j ++) {
            fscanf(file, "%d", &grades[i][j]);
            printf("%d ", grades[i][j]);
            if((j + 1) % (4) == 0) printf("\n");
        }
    }

    // Creating managers and workers to calculate each column
    int n = chapters; // number of managers
    pid_t manager = 0;
    pid_t worker = 0;
    printf("\n[Log] Director creating managers...\n");
    for(int i = 0; i < chapters; i++) { // loop to create managers equal to the number of chapters
        // director process
        manager = fork();
        if(manager < 0) {
            perror("fork");
            exit(1);
        } else if (manager == 0) {
            // manager processes
            int h = homework; // number of workers per manager
            printf("[Log] Manager %d created\n", i + 1);
            for(int j = 0; j < homework; j ++) { // each manager creating workers equal to the number of hw for that chapter
                worker = fork();
                if (worker < 0) {
                    perror("fork");
                    exit(1);
                } else if (worker == 0) {
                    // worker processes
                    printf("[Log] Manager %d's worker %d created \n", i + 1, j + 1);
                    int sum = 0;
                    double avg = 0;
                    for(int s = 0; s < students; s ++) 
                        sum += grades[s][(i*chapters) + j];
                    avg = (double)sum / (double)students;
                    printf("Chapter %d homework %d average: %d / %d = %f\n", i + 1, j + 1, sum, students, avg);
                    exit(0);
                }
            }
            while(h > 0) { // managers waiting for the children
                wait(NULL);
                h--;
            }
            exit(0);
        }
    }
    while(n > 0) { // parent waiting for the managers
        wait(NULL);
        n--;
    }

    // Program exit
    printf("\nDirector reaped all the managers\n");

    printf("\nProgram dead\n");
    return 0;
}