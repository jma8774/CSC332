#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int a = 10, b = 25, fq = 0, fr = 0;
    fq = fork();
    if(fq == 0) {
        a = a + b;
        printf("1: a:%d b:%d fq:%d fr:%d\n", a, b, fq, fr);
        fr = fork();
        if(fr != 0) {
            b = b + 20;
            printf("2: a:%d b:%d fq:%d fr:%d\n", a, b, fq, fr);
        } else {
            a = (a * b) + 30;
            printf("3: a:%d b:%d fq:%d fr:%d\n", a, b, fq, fr);
        }
    } else {
        b = a + b - 5;
        printf("P: a:%d b:%d fq:%d fr:%d\n", a, b, fq, fr);
    }
    return 0;
}