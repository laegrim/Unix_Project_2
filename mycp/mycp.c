#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

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
        exit(EXIT_FAILURE);
    }
    
    if((fd_out=open(target, O_WRONLY | O_CREAT)) == -1)
    {
        printf("cannot open target file.");
        exit(EXIT_FAILURE);
    }
    
    
    // copying files
    while((num_chars=read(fd_in, buffer, BUFFER_SIZE)) > 0)
    {
        if(write(fd_out, buffer, num_chars) != num_chars)
        {
            printf("Error writing to destination\n");
            exit(EXIT_FAILURE);
        }
        
        if(num_chars == -1)
        {
            printf("Error reading from source\n");
            exit(EXIT_FAILURE);
        }
    }
    
    // closing files
    if(close(fd_in) == -1 || close(fd_out) == -1)
    {
        printf("Error closing files.");
        exit(EXIT_FAILURE);
    }
    
    return 1;

}

int isDirectory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}



int main(int argc, char* argv[])
{
    
    char *source;
    char *target;
    
    // checks to see if too many args are given
    if(strncmp(argv[1], "-R", 5) != 0 && argc > 4)
    {
        printf("\nIllegal number of args for -R option\n");
        exit(EXIT_FAILURE);
    }
    
    if(argc != 3)
    {
        printf("\nIllegal number of args\n");
        exit(EXIT_FAILURE);
    }
    
    // checks to see if recursive copy is called, else checks if directories are being copied, else calls regular copy function
    if(strncmp(argv[1], "-R", 5) == 0)
    {
        printf("RECURSIVE CP CALLED\n");
        source = argv[2];
        target = argv[3];
        
        if(isDirectory(source) && isDirectory(target))
        {
            printf("CALL DIRECTORY COPY FUNCTION\n");
        }
        
    }
    else
    {
        source = argv[1];
        target = argv[2];
        copyFiles(source, target);
    }
    exit(1);
}
