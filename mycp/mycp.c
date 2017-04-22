#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

// Ryan Jalufka
// C program to emulate cp command of bash

int copyFiles(char *source, char *target)
{
    int fd_in;
    int fd_out;
    int num_chars;
    char *buffer[BUFFER_SIZE];
    
    
    // opening files
    if((fd_in=open(source, O_RDONLY)) == -1)
    {
        printf("cannot open source file.");
        exit(1);
    }
    
    if((fd_out=open(target, O_WRONLY | O_CREAT)) == -1)
    {
        printf("cannot open target file.");
        exit(1);
    }
    
    
    // copying files
    while((num_chars=read(fd_in, buffer, BUFFER_SIZE)) > 0)
    {
        if(write(fd_out, buffer, num_chars) != num_chars)
        {
            printf("Error writing to destination\n");
        }
        
        if(num_chars == -1)
        {
            printf("Error reading from source\n");
        }
    }
    
    // closing files
    if(close(fd_in) == -1 || close(fd_out) == -1)
    {
        printf("Error closing files.");
    }
    
    return 1;

}




int main(int argc, char* argv[])
{
    if(argc > 4)
    {
      printf("\nIllegal number of args\n");
      exit(EXIT_FAILURE);
    }
    
    
    char *source = argv[1];
    char *target = argv[2];
    
    copyFiles(source, target);
    
    
    if(strncmp(argv[1], "-R", 5) == 0)
    {
        printf("RECURSIVE CP CALLED");
        source = argv[2];
        target = argv[3];
        
    }

    exit(1);
}
