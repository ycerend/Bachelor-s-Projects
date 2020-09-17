/**
 *
 * UNIX pipe program where the parent writes a string to a 1st  pipe.
 * The child process reads this string, reverses the case of each 
 * character, and writes its result back to a 2nd pipe.
 * The parent process then reads this reversed result from the 2nd pipe.
 */

/*
 *****
 *****
 *****
 This is a simple pipe program with 2 separate pipes.
 The parent process writes a string from the 1st pipe,
 which is then read by the child process through the same pipe.
 Then, the child process reverses the case of each character received.
 The reversed version is sent through the 2nd pipe to the parent process.
 *****
 *****
 *****
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 20
#define READ_END	0
#define WRITE_END	1

int main(void)
{
	char write_msg[BUFFER_SIZE] = "Greetings";
	char read_msg[BUFFER_SIZE];
	pid_t pid;
	int first_pipe[2];
	int second_pipe[2];
	int i;

	if (pipe(first_pipe) == -1) {
		fprintf(stderr,"First Pipe failed");
		return 1;
	}

 	if (pipe(second_pipe) == -1) {
                fprintf(stderr,"Second Pipe failed");
                return 1;
        }


	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}

	if (pid > 0) {
		printf("2\n");
		// Writing to the pipe
		write(first_pipe[WRITE_END], write_msg, strlen(write_msg)+1); 
		printf("3\n");
		// Closing the write end of the first pipe
		close(first_pipe[WRITE_END]);
		if ( wait(0) == -1){
			perror("waiting\n");
		}
		printf("pid number %d\n",pid);
		printf("1\n");
		// Closing the unused ends
		close(first_pipe[READ_END]);
		close(second_pipe[WRITE_END]);


		printf("4\n");
		// Reading the message from child through second pipe
		read(second_pipe[READ_END], read_msg, BUFFER_SIZE);
		printf("parent read %s\n",read_msg);
        // Closing the last end
		close(second_pipe[READ_END]);
	}
	else {
		printf("5\n");
		// Closing the unused ends
		close(first_pipe[WRITE_END]);
		close(second_pipe[READ_END]);
		printf("6\n");
		// Read the message sent by the parent
		read(first_pipe[READ_END], read_msg, BUFFER_SIZE);
		printf("child read %s\n",read_msg);

		// Reversing the string received
		for (i = 0; i < strlen(read_msg); i++) {
			if (isupper(read_msg[i]))
				write_msg[i] = tolower(read_msg[i]);
			else if (islower(read_msg[i]))
				write_msg[i] = toupper(read_msg[i]);
			else
				write_msg[i] = read_msg[i];
		}
		printf("7\n");
		// Writing back to the parent
		write(second_pipe[WRITE_END], write_msg, strlen(write_msg)+1);
		printf("8\n");

		// Closing the remaining ends
		close(first_pipe[READ_END]);
		close(second_pipe[WRITE_END]);
		printf("9\n");
	}

	return 0;
}
