#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>

#define BUFFER_SIZE 1024
extern int errno;

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



void recursiveTraverse(const char *source, const char *target){
    DIR *dirp = opendir(source);
    if(dirp)
    {
        char currentPath[PATH_MAX];
        char buffer[PATH_MAX];
        struct dirent *entry;
        while((entry = readdir(dirp)) != NULL) {
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            struct stat info;
	    chdir(source);
	    stat(entry->d_name, &info);
            if(S_ISDIR(info.st_mode))
            {
                //make corresponding directory in target folder here
                //mkdir(target);
                sprintf(currentPath, "%s/%s", getcwd(buffer, PATH_MAX), entry->d_name);
                printf("Current path to traverse: %s\n", currentPath);
                recursiveTraverse(currentPath, target);
            }
            else if(S_ISREG(info.st_mode))
            {
                DIR *dirp2 = opendir(target);
                struct dirent *targetEntry;
                copyFiles(entry->d_name, target);
                //Run regular file copy routine to target folder
                printf("File copied...\n");
            }
        }
    }
}




int main(int argc, char* argv[])
{
    printf("argv[1]: is %s\n", argv[1]);
    
    char *source;
    char *target;
    
    // checks to see if too many args are given
    if(strncmp(argv[1], "-R", 5) != 0 && argc > 4)
    {
        printf("\nIllegal number of args for -R option\n");
        exit(EXIT_FAILURE);
    }
    
    // checks to see if recursive copy is called, else checks if directories are being copied, else calls regular copy function
    if(strncmp(argv[1], "-R", 5) == 0)
    {
        printf("RECURSIVE CP CALLED\n");
        source = argv[2];
        target = argv[3];
        
        recursiveTraverse(source, target);
    }
    else
    {
        source = argv[1];
        target = argv[2];
        copyFiles(source, target);
    }
    exit(1);


}
