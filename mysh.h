/* Various helper functions for the shell implementation
 */

#ifndef _MYSH_H_
#define _MYSH_H_

void check_for_redirect_in(char ** line_tokens){
	//check for and handle the redirection token <

}

void check_for_redirect_out(char ** line_tokens){
	//check for and handle the redirection token >
}

void check_for_pipe(char ** line_tokens){
	//Check for and handle the pipe token |
}

void check_for_errors(char ** line_tokens){
	//Check the line for any obvious errors
}

//This function handles the execution
void exec_line(char ** line_tokens){

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
		//check for pipe token (|)
		check_for_pipe(line_tokens);
		//execute the commands
	}
	// if we are the parent, then wait for the child to finish
	else {
		
	}
	
}
