#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
//#include <"mysh.h">
//#include <"token.h">
#define MAX_BUF 512
#define MAX_ARGS 10
#define DLM_SPACE " "

int num_args;

void parse(char* cmd, char** args)
{
    int i = 0;
    while(args[i] != NULL){ args[i] = strsep(&cmd, " "); i++;}
    num_args = i;
}

int myexec(char **args)
{

    pid_t pid, wpid;
    int status;

    pid = fork();
    
    if(pid == 0) 
    {
        if(execvp(args[0], args) == -1)
        {
                printf("ERROR: EXECVP\n");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid < 0){printf("ERROR: FORKING\n"); exit(EXIT_FAILURE);}
    else
    {
       waitpid(pid, &status, WUNTRACED);
       return 1;
    }
    return 0;
}


int main(int argc, char **argv)
{
    char cmd[MAX_BUF + 1];
    char *args[MAX_ARGS + 1];

    while(1)
    {
        char* dir = getenv("PWD");
        printf("%s$ ", dir);

        if(fgets(cmd, sizeof(cmd), stdin) == NULL) break;
        
        if(cmd[strlen(cmd) - 1] == '\n') cmd[strlen(cmd) - 1] = '\0';

        fprintf(stdout, "%s", cmd); 
        
        parse(cmd, args);

        if(strcmp(args[0], "exit") == 0) break;

        if(myexec(args) == 0) break;
    }
    return 0;
}
    
    

