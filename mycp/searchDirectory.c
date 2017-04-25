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


void recursiveTraverse(const char *absPath, const char *target){
    DIR *dirp = opendir(absPath);
    if(dirp)
    {
        char Path[PATH_MAX];
        char *EndPtr = Path;
        struct dirent *entry;
        strcpy(Path, absPath);
        EndPtr += strlen(absPath);
        while((entry = readdir(dirp)) != NULL) {
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            struct stat info;
            strcpy(EndPtr, entry->d_name);
            printf("EndPtr Value: %20s\n", EndPtr);
            if(!stat(Path, &info))
            {
                printf("Path Value: %20s\n", Path);
                if(S_ISDIR(info.st_mode))
                {
                    //make corresponding directory in target folder here
                    mkdir(target, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                    recursiveTraverse(Path, target);
                }
                else if(S_ISREG(info.st_mode))
                {
                    //Run regular file copy routine to target folder
                    printf("Found regular file!\n");
                }
            }
        }
    }
}

int getFilesInDir(const char* source)
{
    DIR* dirp;
    struct dirent* direntp = NULL;
    
    struct stat s;
    
    dirp = opendir(source);
    if( dirp == NULL ) {
        perror( "can't open...");
    } else {
        while ((direntp = readdir(dirp)))
        {
            if(direntp == NULL)
            {
                printf ("ERROR! direntp could not be initialised correctly");
                exit (3);
            }
            else {
                printf ("FILE CONTENT: %s\n", direntp->d_name);
            }
            
        }
        
    }

    return EXIT_SUCCESS;

}


int main(int argc, char* argv[])
{
    
    printf("STARTING MAIN... \n");
    printf("argv[1]: is %s\n", argv[1]);
   // SearchDirectory(argv[1]);
 
    const char* source = argv[1];
    const char* target = argv[2];
 
    getFilesInDir(source);
    printf("\nCalling recursive Traverse...\n");
    recursiveTraverse(source, target);
    
    return 0;
}
