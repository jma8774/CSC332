#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
int main (int argc, char* argv[])
{
char* filepath = argv[1];
int returnval;
// A
// Check file existence
returnval = access (filepath, F_OK);
if (returnval == 0)
    printf ("File %s exists\n", filepath);
else {
    if (errno == ENOENT)
    printf ("File %s does not exist\n", filepath);
    else if (errno == EACCES)
    printf ("File %s is not accessible\n", filepath);
    return 0;
}

// Check read access ...
// Check write access ...

returnval = access (filepath, R_OK);
if (returnval == 0)
    printf ("File %s is readable\n", filepath);
else {
    printf ("File %s is not readable\n", filepath);
}

returnval = access (filepath, W_OK);
if (returnval == 0)
    printf ("File %s is writeable\n", filepath);
else {
    printf ("File %s is not writeable\n", filepath);
}

// B
// int fd = creat("destination.txt", O_RDONLY);
int fd = open("destination.txt", O_RDONLY | O_CREAT);
if(-1 == fd) {
    printf("Command open() failed with error [%s]\n", strerror(errno));
    return 1;
} else {
    printf("File opened successfully\n");
}
return 0;
}