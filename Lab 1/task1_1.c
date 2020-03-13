#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
int main (int argc, char* argv[])
{
char* filepath = argv[1];
int returnval;
// Check file existence
returnval = access (filepath, F_OK);
if (returnval == 0) {
    printf ("File %s exists\n", filepath);
    returnval = access (filepath, R_OK);
    if (returnval == 0)
        printf ("File %s is readable\n", filepath);
    else {
        printf ("File %s is not readable\n", filepath);
        return 0;
    }
} else {
    if (errno == ENOENT)
    printf ("File %s does not exist\n", filepath);
    else if (errno == EACCES)
    printf ("File %s is not accessible\n", filepath);
    return 0;
}

int fd = open(filepath, O_RDONLY);
int s = lseek(fd, 0, SEEK_END); 			// s is the size of the file
char *buffer = malloc(s); 				// allocate memory for buffer
lseek(fd, 0, SEEK_SET); 				// going back to the beginning
printf("Size of file is %d bytes\n", s);
read(fd, buffer, s); 					// reading from beginning to end
printf("Displaying file info:\n%.*s\n", s, buffer); 	// displaying what is read into the buffer
return 0;
}
