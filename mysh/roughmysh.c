#include <stdio.h>
#include <stdio.lib>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define TOKEN_LENGTH 100;

//Function Declarations for shell commands
int mycd(char **input);
int pwd(char **input);

char *builtin_commands[] = {"mycd", "pwd"};

int mycd(char **input) {

}

int pwd(char **input) {

}

int launch_command(char **input) {
	pid_t pid = fork();
	int childstatus;
	
	if (pid < 0) {
		//Error forking
		perror("Forking Error");
		exit(-1);
	}else if (pid == 0) {
		//Child process
		execvp(input[0], input);
	} else {
		do {
      			wpid = waitpid(pid, &status, WUNTRACED);
    		} while (!WIFEXITED(status) && !WIFSIGNALED(status))
	}
	return 1;
}

#define READ_LINE_LENGTH 1000;
char *read_line(void) {
	int buffer_size = READ_LINE_LENGTH;
	int position = 0;
	char *buffer = malloc(sizeof(char) * buffer_size);
	int c;

	if (!buffer) {
    		perror ("Nothing to read");
    		return 1;
  	}

 	 while (1) {
   		 // Read a character
   		 c = getchar();

   		 // If we hit EOF, replace it with a null character and return.
   		 if (c == EOF || c == '\n') {
     		 buffer[position] = '\0';
     		 return buffer;
   	 } else {
     		 buffer[position] = c;
   	 }
   	 position++;

   	 // If we have exceeded the buffer, reallocate.
   	 if (position >= bufsize) {
      		bufsize += LSH_RL_BUFSIZE;
     		 buffer = realloc(buffer, bufsize);
     	 if (!buffer) {
       		 fprintf(stderr, "lsh: allocation error\n");
       		 exit(EXIT_FAILURE);
     	 }
    }	
}

#define TOKEN_LENGTH 100;
char **tokenize(**input) {
	int bufsize = MYSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;
	//Create tokens
	return tokens;
}
int main (int argc, char **argv) {
	char *line;
	char **input;
	
	while(1) {
		char *directory = getenv("PWD");
		printf("%sshell > ", directory);

		line = read_line();
    		input = tokenize(line);
    		status = ?
	}
	return 0;
}
