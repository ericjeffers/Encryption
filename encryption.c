/*
Eric Jeffers
CSC 565
Assignment 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
 
#define BUF_SIZE 4096
 
int main(int argc, char* argv[]) {
 
	int input_fd, key_fd, output_fd;    /* Input, key, and output file descriptors */
	ssize_t ret_in, ret_key, ret_out;    /* Number of bytes returned */
	char inBuffer[BUF_SIZE], keyBuffer[BUF_SIZE];      /* Character buffers */
	int i, j;    /* Counters for loops */
 
	/* Create input file descriptor */
	input_fd = open (argv [1], O_RDONLY);

	/* Create key file descriptor */
	key_fd = open (argv [2], O_RDONLY);
 
	/* Delete duplicate file */
	remove(argv[3]);
	/* Create output file descriptor */
	output_fd = open(argv[3], O_WRONLY | O_CREAT, 0644);

	/* Encrypt process */
	while((ret_in = read (input_fd, &inBuffer, BUF_SIZE)) > 0){    /* Loop by 4kb chunks until end */
		ret_key = read (key_fd, &keyBuffer, BUF_SIZE);
		j = 0;
		for (i = 0; i < sizeof (ret_in); i++){    /* Loop back through key if it ends early */
			if (j > sizeof (ret_key))
				j = 0;
			inBuffer[i] ^= keyBuffer[j];    /* Bitwise XOR for encryption */
			j++;
		}
		ret_out = write (output_fd, &inBuffer, (ssize_t) ret_in);    /* Write encrypted content */
		if(ret_out != ret_in){
			/* Write error if output is different length*/
			perror("write");
			return 2;
		}
	}
 
	/* Close file descriptors */
	close (input_fd);
	close (key_fd);
	close (output_fd);
 
	return (EXIT_SUCCESS);
}
