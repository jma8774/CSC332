#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
    int fd = open("destination1.txt", O_RDWR | O_CREAT, 0777);
    close(fd);

    fd = open("destination2.txt", O_RDWR | O_CREAT, 0777);
    close(fd);

    printf("Destination1.txt and destination2.txt has been successfully created\n");
    return 0;
}