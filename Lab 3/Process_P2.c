#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
    int s = open("source.txt", O_RDONLY);
    int d1 = open("destination1.txt", O_RDWR);
    int d2 = open("destination2.txt", O_RDWR);
    
    int s_size = lseek(s, 0, SEEK_END);
    lseek(s, 0, SEEK_SET);
    printf("Size of source.txt is %d bytes\n", s_size);

    char buf1[100];
    char buf2[50];
    int turn = 1;
    int temp = s_size;
    while(temp > 0) {
        if(turn == 1) {
            if(temp - 100 >= 0) {
                read(s, buf1, 100);
                write(d1, buf1, 100);
            } else {
                char rem_buff[temp];
                read(s, rem_buff, temp);
                write(d1, rem_buff, temp);
            }
            temp -= 100;
            turn = 2;
        } else {
            if(temp - 50 >= 0) {
                read(s, buf2, 50);
                write(d2, buf2, 50);
            } else {
                char rem_buff[temp];
                read(s, rem_buff, temp);
                write(d2, rem_buff, temp);
            } 
            temp -= 50;
            turn = 1;
        }
    }

    printf("Finished writing onto destination1.txt and destination2.txt... \n");

    int d1_size = lseek(d1, 0, SEEK_END);
    lseek(d1, 0, SEEK_SET);
    printf("Size of destination1.txt file is %d bytes\n", d1_size);

    int d2_size = lseek(d2, 0, SEEK_END);
    lseek(d2, 0, SEEK_SET);
    printf("Size of destination2.txt file is %d bytes\n", d2_size);

    close(s);
    close(d1);
    close(d2);
    return 0;
}