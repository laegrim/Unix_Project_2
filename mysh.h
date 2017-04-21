/* Various helper functions for the shell implementation
 */

#ifndef _MYSH_H_
#define _MYSH_H_

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


void check_for_redirect_in(char ** line_tokens){
	//check for and handle the redirection token <

}

void check_for_redirect_out(char ** line_tokens){
	//check for and handle the redirection token >
}

bool check_for_pipe(char ** line_tokens, char *** pipe_segments, int * p){
	//Check for and handle the pipe token |

}

void check_for_errors(char ** line_tokens){
	//Check the line for any obvious errors
}

void handle_error(int &status){
}


//This function handles the execution
void exec_line(char ** line_tokens){
	
	pid_t pid;
	int status;
	char *** pipe_segments;
	bool is_pipe;
	int p[2];

	//check tokens for a pipe
	is_pipe = check_for_pipe(line_tokens, pipe_segments, p);
	
	//if there's a pipe, then we need to execute pipe segements and redirect 
	//the output from one segment to the input of the next
	if (is_pipe){
		
		int segment = 0;

		//while there are still pipe segments (must be at least 2)
		while (pipe_segments[segment] != NULL) {
			
			//fork a child process
			pid = fork();

			//if you are the child
			if (pid == 0) {
				
				//if you aren't the last, push stdout throught the pipe
				if (pipe_segments[segment + 1] != NULL) {dup2(p[1], 1);}
				//if you aren't the first, pull stdin from the pipe
				if (segment > 0) {dup2(p[0], 0);}
				
				close(p[0])
				close(p[1])

				//get your pipe segment
				command = pipe_segments[segment];
				//check for any errors in the command
				check_for_errors(command);
				//check for any redirection (takes precedence over the pipe)
				check_for_redirect_in(command);
				check_for_redirect_out(command);
				//execute the command
				execvp(command[0], command[1]);
			}
			//if you are the parent, wait for the child to finish
			else if (pid > 0) {
				pid = wait(pid, &status, 0);
			}
			//handle any errors from the forking
			else {
				handle_error(pid);
			}
		}
		//close the pipe
		close(p[0]);
		close(p[1]);	
	}
	else {
		//fork a child process
		pid = fork();
		//if we are the child, then
		if (pid == 0){ 
			//do some error checking on the tokens
			check_for_errors(line_tokens);
			//check for redirection token (<)
			check_for_reidrect_in(line_tokens);
			//check for redirection token (>)
			check_for_redirect_out(line_tokens);
			//execute the commands
			execvp(line_tokens[0], line_tokens[1]);
		}
		// if we are the parent, then wait for the child to finish
		else if (pid > 0){
			pid = waitpid(pid, &status, 0)
		}
		//if the pid is < 0, then there was an error forking; handle it
		else {
			handle_error(pid)
		}
	}
}
