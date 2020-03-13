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

// Open destination in write-only...
int fd2 = open(destination, O_WRONLY);
if(-1 == fd) {
    printf("Command open() failed with error [%s]\n", strerror(errno));
    return 1;
} else {
    printf("Destination opened successfully in write-only\n");
}

// Copying content from source to destintation...
int s = lseek(fd, 0, SEEK_END);
lseek(fd, 0, SEEK_SET);
printf("Size of source file is %d bytes\n", s);


// Writing on destination.txt...
int loops = s/100;
int remainder = s%100;
char* xyzbuff = "XYZ";
char buffer[100];

for(int i = 0; i < loops; i++) {
	read(fd, buffer, 100);
	for(int j = 0; j < 100; j++) {
		if (buffer[j] == '1') buffer[j] = 'A';	// replacing '1' with 'A'
	}
	write(fd2, buffer, 100);			// writing 100 chars
	write(fd2, xyzbuff, 3);				// writing xyz
}

// Writing last 100 chars...
char leftover[remainder];
read(fd, leftover, remainder);
for(int i = 0; i < remainder; i ++) {
	if(leftover[i] == '1') leftover[i] = 'A';	// replacing '1' with 'A'
}
write(fd2, leftover, remainder);			// writing leftover chars
write(fd2, xyzbuff, 3);					// writing xyz

printf("Successfully wrote onto destination.txt\n");
close(fd);
close(fd2);
printf("Closing source.txt file\n");
printf("Closing destination.txt file\n");

return 0;
}
