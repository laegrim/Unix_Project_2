#include <sys/param.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

extern int errno;
static char * err_str;

static int read_input_to_output(int input_fd, char * filename){

	//get stdout
	int output_fd = fileno(stdout);
	
	//setup a buffer
	static char * buffer = NULL;

	//do some initialization before reading
	struct stat fd_info;
	ssize_t bytes_read, bytes_written = 0;
	int offset; 

	//get information about fd
	if (fstat(output_fd, &fd_info)) {//err
		err_str = (char *) malloc(sizeof(char) * 100);
		sprintf(err_str, "mycat: %s", filename);
		perror(err_str);
		free(err_str);
		exit(1);
	}
	//set the blocksize
	size_t blocksize = MAX(fd_info.st_blksize, 1024);
	//allocate the buffer to be the blocksize
	if ((buffer = (char *) malloc(blocksize)) == NULL) {//err
		perror("mycat: buffer");
		exit(1);
	}
	
	//start the read write loop
	//while there's data to read from the input file, read it
	while ((bytes_read = read(input_fd, buffer, blocksize)) > 0){
		//keep track of the difference between the ammount of data read
		//and the ammount of data written
		offset = 0;
		while (bytes_read > 0) {
			if ((bytes_written = write(output_fd, buffer + offset, (size_t) bytes_read)) < 0) {//err
				free(buffer);
				perror("stdout");
				exit(1);
			}
			bytes_read -= bytes_written;
			offset += bytes_written;
		}
	}

	if (bytes_read < 0) {//err
		free(buffer);
		err_str = (char *) malloc(sizeof(char) * 100);
		sprintf(err_str, "mycat: %s", filename);
		perror(err_str);
		free(err_str);
		return 1;
	}
	free(buffer);	
}

int main(int argc, char ** argv){
	
	//determine where the input is coming from
	//stdout vs file
	//there might also be multiple inputs
	
	int input_fd;
	int status = 0;
	
	//if argc is 1, then there's been no file provided, input must be from stdin
	if (argc == 1) {
		input_fd = STDIN_FILENO;
		status = read_input_to_output(input_fd, "stdin");
	}
	else {
		for (int i = 1; i < argc; i++) {
			if((input_fd = open(argv[i], O_RDONLY)) < 0) {//err
				err_str = (char *) malloc(sizeof(char) * 100);
				sprintf(err_str, "mycat: %s", argv[i]);
				perror(err_str);
				free(err_str);
				status = 1;
			}
			else {
				status = read_input_to_output(input_fd, argv[i]);
				close(input_fd);
			}
		}
	}
	//attempt to close output stream
	if (fclose(stdout)) {//err
		perror("mycat: stdout");
		exit(1);
	}
	//exit gracefully
	exit(status);
}	
