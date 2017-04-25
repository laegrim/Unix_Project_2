/* Various helper functions for the shell implementation
 */

#ifndef _MYSH_H_
#define _MYSH_H_

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


void check_for_redirect_in(char ** line_tokens, int token_count){
	//check for and handle the redirection token <
	
	//loop through and look at each token
	for (int i = 0; i < token_count; i++) {
		//if the token is a stdin redirection token
		if(line_tokens[i] == '<') {
			//the immediate right hand side token is the source of the redirect
			//get a file descriptor for that
			if (i == token_count - 1) {
				//we have an error, next token is null terminator
			}
			char * filepath = line_tokens[curr_tok_idx + 1];
			if (int source_fd = open(filepath, O_WRONLY|O_CREAT|O_TRUNC, 0600)) {
				//we have an error
			}
			//duplicate the file descriptor so that it is at stdin
			if (dup2(source_fd, 0) < 0){
				//we have an error
			}
			//close the old file descriptor
			close(source_fd);
			//remove the redirection token and redirection source from the line
			char ** tmp = (char **) malloc(sizeof(char *) * (token_count - 2));
			for (int j = 0; j < i; j++) {tmp[j] = line_tokens[j];}
			for (int j = i + 2; j < token_count; j++) {tmp[j - 2] = line_tokens[j];}
		        line_tokens = tmp;
			break;	
		}
	}
}

void check_for_redirect_out(char ** line_tokens, int token_count){
	//check for and handle the redirection token >
	
	//loop through and look at each token
	for (int i = 0; i < token_count; i++) {
		//if the token is a stdin redirection token
		if(line_tokens[i] == '>') {
			//the immediate right hand side token is the source of the redirect
			//get a file descriptor for that
			if (i == token_count - 1) {
				//we have an error, next token is null terminator
			}
			char * filepath = line_tokens[curr_tok_idx + 1];
			if (int source_fd = open(filepath, O_WRONLY|O_CREAT|O_TRUNC, 0600)) {
				//we have an error
			}
			//duplicate the file descriptor so that it is at stdin
			if (dup2(source_fd, 1) < 0){
				//we have an error
			}
			//close the old file descriptor
			close(source_fd);
			//remove the redirection token and redirection source from the line
			char ** tmp = (char **) malloc(sizeof(char *) * (token_count - 2));
			for (int j = 0; j < i; j++) {tmp[j] = line_tokens[j];}
			for (int j = i + 2; j < token_count; j++) {tmp[j - 2] = line_tokens[j];}
		        line_tokens = tmp;
			break;	
		}
	}

}

bool check_for_pipes(char ** line_tokens, int token_count, char *** pipe_segments, &num_segments){
	//Check for and handle the pipe token |
}

void check_for_errors(char ** line_tokens, int token_count){
	//Check the line for any obvious errors
}

//This function handles the execution
void exec_line(char ** line_tokens, int token_count){
	
	pid_t pid;
	int status;
	char *** pipe_segments;
	int num_segments;
	
	//if there's a pipe, then we need to execute pipe segements and redirect 
	//the output from one segment to the input of the next
	if (check_for_pipes(line_tokens, token_count, pipe_segments, num_segments)){
		
		int segment = 0;

		//create all the pipes needed up front
		int ** pipes = malloc(sizeof(int) * 2 * (num_segments));
		for (int i = 0; i < num_segments; i ++) {
			pipes[i] = int[2];
			pipe(pipes[i]);
		}


		//while there are still pipe segments (must be at least 2)
		while (pipe_segments[segment] != NULL) {
			
			//fork a child process
			pid = fork();

			//if you are the child
			if (pid == 0) {
				
				int * p = pipes[segment]	
				//if you aren't the last, push stdout throught the pipe
				if (pipe_segments[segment + 1] != NULL) {dup2(p[1], 1);}
				//if you aren't the first, pull stdin from the pipe
				if (segment > 0) {dup2(p[0], 0);}
				
				close(p[0])
				close(p[1])

				//get your pipe segment
				command = pipe_segments[segment];
				command_tokens = 0;
				do {command_tokens++;} while (command[command_tokens] != NULL);
				//check for any errors in the command
				check_for_errors(command, command_tokens);
				//check for any redirection (takes precedence over the pipe)
				check_for_redirect_in(command, command_tokens);
				check_for_redirect_out(command, command_tokens);
				//execute the command
				execvp(command[0], command[1]);
			}
			//if you are the parent, wait for the child to finish
			else if (pid > 0) {
				pid = wait(pid, &status, 0);
			}
			//handle any errors from the forking
			else {
			}
		}
		//close all of the pipes
		for (int i = 0; i < num_segments; i++){
			close(pipes[i][0]);
			close(pipes[i][1]);
		}
	}
	else {
		//fork a child process
		pid = fork();
		//if we are the child, then
		if (pid == 0){ 
			//do some error checking on the tokens
			check_for_errors(line_tokens, token_count);
			//check for redirection token (<)
			check_for_reidrect_in(line_tokens, token_count);
			//check for redirection token (>)
			check_for_redirect_out(line_tokens, token_count);
			//execute the commands
			execvp(line_tokens[0], line_tokens[1]);
		}
		// if we are the parent, then wait for the child to finish
		else if (pid > 0){
			pid = waitpid(pid, &status, 0);
		}
		//if the pid is < 0, then there was an error forking; handle it
		else {
		}
	}
}
