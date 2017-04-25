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
                mkdir(target, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                sprintf(currentPath, "%s/%s", getcwd(buffer, PATH_MAX), entry->d_name);
                printf("Current path to traverse: %s\n", currentPath);
                recursiveTraverse(currentPath, target);
            }
            else if(S_ISREG(info.st_mode))
            {
                copyFiles(currentPath, target);
                //Run regular file copy routine to target folder
                printf("File copied...\n");
            }
        }
    }
}




int main(int argc, char* argv[])
{
    
    printf("STARTING MAIN... \n");
    printf("argv[1]: is %s\n", argv[1]);
    
    const char* source = argv[1];
    const char* target = argv[2];
    
    printf("\nCalling recursive Traverse...\n");
    recursiveTraverse(source, target);
    
    return 0;
}
