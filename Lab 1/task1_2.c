#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
int main (int argc, char* argv[])
{
char* source = argv[1];
char* destination = argv[2];
int returnval;

// Check file existence
returnval = access(source, F_OK);
if (returnval == 0)
    printf ("Source file %s exists\n", source);
else {
    if (errno == ENOENT)
    printf ("Source file %s does not exist\n", source);
    else if (errno == EACCES)
    printf ("Source file %s is not accessible\n", source);
    return 0;
}

// Check read access of source...
returnval = access (source, R_OK);
if (returnval == 0)
    printf ("Source file %s is readable\n", source);
else {
    printf ("Source file %s is not readable\n", source);
    return 0;
}

// Open source in read-only...
int fd = open(source, O_RDONLY);
if(-1 == fd) {
    printf("Command open() failed with error [%s]\n", strerror(errno));
    return 1;
} else {
    printf("Source opened successfully in read-only\n");
}

// Copying content from source to buffer...
int s = lseek(fd, 0, SEEK_END);
lseek(fd, 0, SEEK_SET);
char *buffer = malloc(s);
printf("Size of source file is %d bytes\n", s);
read(fd, buffer, s);
printf("Displaying file info:\n%.*s\n", s, buffer);
close(fd);
printf("Closing source file\n\n\n\n");

// Open destination in write-only...
fd = open(destination, O_WRONLY);
if(-1 == fd) {
    printf("Command open() failed with error [%s]\n", strerror(errno));
    return 1;
} else {
    printf("Destination opened successfully in write-only\n");
}

// Writing what was read into the buffer onto the destination.txt
printf("Successfully wrote onto destination.txt\n");
write(fd, buffer, s);
printf("Closing destination.txt\n");
close(fd);
return 0;
}
