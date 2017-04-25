#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#define MAX_BUF 512

int num_args, num_toke;
int rdirin = 1, rdirout = 1, pipefl = 1, mycdfl = 1;

int mycat(char** args);
int mycp(char** args);
int myls(char** args);
int mycd(char** args);
int mypwd(char** args);

char *builtin_str[] = { "mycat", "mycp", "myls", "mycd", "mypwd" };

int (*builtin_func[]) (char**) = { &mycat, &mycp, &myls, &mycd, &mypwd };

int num_builtins()
{
    return sizeof(builtin_str) / sizeof(char*);
}

int mycat(char** args){}
int mycp(char** args){}
int myls(char** args){}
int mycd(char** args)
{
    char path[MAX_BUF];
    strcpy(path, args[1]);

    mycdfl = 0;
    char cwd[MAX_BUF + 1];
    if(path[0] != '/')
    {
        getcwd(cwd, sizeof(cwd));
        strcat(cwd, "/");
        strcat(cwd, path);
        chdir(cwd);
    }
    else 
    {
        chdir(path);
    }
    return 1;
}
int mypwd(char** args){}

int check_builtins(char** args)
{
    for(int i = 0; i < num_builtins(); i++)
    {
        if(strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }
    return 1;
}

char** parse(char* cmd)
{
    num_args = 0;
    char *tok;
    char **targs = malloc(sizeof(char*) * sizeof(cmd + 1));
    tok = strtok(cmd, " <>|");
    int i = 0;
    while(tok != NULL)
    {
        targs[i] = tok;
        i++;
        tok = strtok(NULL, " <>|"); 
    }
    targs[i] = NULL;
    num_args = i;
    return targs;
}

void del (char str[], char ch) 
{
    int i, j = 0;
    int size;
    char ch1;
    char str1[10];

    size = strlen(str);

    for (i = 0; i < size; i++) {
        if (str[i] != ch) {
            ch1 = str[i];
            str1[j] = ch1;
            j++;
        }
    }
    str1[j] = '\0';
    strcpy(str, str1);
}


char** tokenizer(char* cmd)
{
    num_toke = 0;
    char *token;
    char **toargs = malloc(sizeof(char*) * sizeof(cmd + 1));
    token = strtok(cmd, "<>|");
    int i = 0;
    while(token != NULL)
    {
        del(token, ' ');
        toargs[i] = token;
        i++;
        token = strtok(NULL, "<>|"); 
    }
    toargs[i++] = NULL;
    num_toke = i - 1;
    return toargs;
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

int myredir(char** targs, char** args )
{   
    pid_t pid, wpid;
    char** arg_one, arg_two, arg_three, nargs;
    int status;
    int in;
    int out;

    pid = fork();

    if(pid == 0) 
    {
        if(rdirin == 0)
        {
            
            in = open(targs[1], O_RDWR);
            dup2(in, 0);
            close(in);
        }
        if(rdirout == 0)
        {
            if(num_toke > 2)
            {
                out = creat(targs[2], O_RDONLY);
                dup2(out, 1);
                close(out);
            }
            else
            {
                out = open(targs[1], O_RDWR);
                dup2(out, 1);
                close(out);
            }
        }
        execvp(args[0], args);   

        
    }
    else if(pid < 0){printf("ERROR: FORKING\n"); exit(EXIT_FAILURE);}
    else
    {
       waitpid(pid, &status, WUNTRACED);
       return 1;
    }
    return 0;
}

int mypipe(char** args)
{
    pid_t pid, wpid;
    char** arg_one;
    char** arg_two;
    int status;
    int p[2];
    pipe(p);
    
    arg_one = parse(args[0]);
    arg_two = parse(args[1]);

    pid = fork();
    
    if(pid == 0) 
    {
        close(p[0]);
        dup2(p[1], 1);
        close(p[1]);

        if(execvp(arg_one[0], arg_one) == -1)
        {
            printf("ERROR: EXECVP\n");
        }
        return 1;
    }
    else if(pid < 0){printf("ERROR: FORKING\n"); exit(EXIT_FAILURE);}
    else
    {
       close(p[1]);
       dup2(p[0], 0);
       close(p[0]);

       waitpid(pid, &status, WUNTRACED);

       if(execvp(arg_two[0], arg_two) == -1)
       {
           printf("ERROR: EXECVP1\n");
       }
       return 1;
    }
    return 1;
}


int pipefork(char** args)
{
    int pid, status;
    pid = fork();

    if(pid == 0)
    {
        mypipe(args);
    }
    else if(pid < 0)
    {
        return 0;
    }
    else
        pid = wait(&status);
    return 1;
}

void sredir(char* line)
{
    for(int i = 0; i < strlen(line); i++)
    {
        if(line[i] == '<')
        {
            rdirin = 0;
        }
        else if(line[i] == '>')
        {
            rdirout = 0;
        }
        else if(line[i] == '|')
        {
            pipefl = 0;
        }
        else;
    }
}

int main(int argc, char **argv)
{
    char *cmd = NULL;
    char **args;
    char **targs;
    ssize_t bufs = 0;

    while(1)
    {
        char* dir = getenv("PWD");
        printf("%s$ ", dir);

        getline(&cmd, &bufs, stdin);
        
        sredir(cmd);
        if(strlen(cmd) > 1)
        {
            if(cmd[strlen(cmd) - 1] == '\n') cmd[strlen(cmd) - 1] = '\0'; 
           
            //if(check_builtins(args) == 0) break;
            if(rdirin == 0)
            {
                targs = tokenizer(cmd);
                args = parse(cmd);
                if(myredir(targs, args) == 0) break;
            }
            else if(rdirout == 0)
            {
                targs = tokenizer(cmd);
                args = parse(cmd);
                if(myredir(targs, args) == 0) break;
            }
            else if(pipefl == 0)
            {
                targs = tokenizer(cmd);
                args = parse(cmd);
                if(pipefork(args) == 0) break;
            }
            else if(mycdfl == 0)
            {
                mycdfl = 1;
            }
            else
            {
                args = parse(cmd);
                if(strcmp(args[0], "exit") == 0) break;
                if(check_builtins(args) == 0) break;
                if(myexec(args) == 0) break;
                
            }

            rdirin = 1;
            rdirout = 1;
            pipefl = 1;
        }
    }
    free(args);
    free(targs);
    return 0;
}
    
    

