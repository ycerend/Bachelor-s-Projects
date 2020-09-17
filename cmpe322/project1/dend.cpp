#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>  
#include <iostream>

using namespace std;

#define BUFFER_SIZE 100
#define READ_END	0
#define WRITE_END	1

int main() {
    char buf;
    char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE];
    pid_t pid;
    int first_pipe[2];
    int second_pipe[2];
    int third_pipe[2];
    int i;
    char command;
    
    if (pipe(first_pipe) == -1) {
        fprintf(stderr,"First Pipe failed");
        return 1;
    }

    if (pipe(second_pipe) == -1) {
        fprintf(stderr,"Second Pipe failed");
        return 1;
    }
    if (pipe(third_pipe) == -1) {
        fprintf(stderr,"Third Pipe failed");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }  
        
    if (pid == 0) {    //Child process  
        read(first_pipe[READ_END], read_msg, BUFFER_SIZE);
        printf("child read1 %s\n",read_msg);
        write(second_pipe[WRITE_END], "geldik   " , strlen(write_msg)+1);
    
    }
    else {  //Parent process
        do{
        cout << "Enter a command (q, u, p, t): ";   
        cin >> buf; 
        
        write(first_pipe[WRITE_END], &buf, 1);
        if ( wait(0) == -1){
            perror("waiting\n");
        }
        }while(buf != 't');
        if(buf == 't'){
            cout << "\nExiting program...";
            close(first_pipe[1]);
            close(first_pipe[0]);
            exit(EXIT_SUCCESS);
        }
    }  
}



    /*char write_msg[BUFFER_SIZE] = "Greetings";
	char read_msg[BUFFER_SIZE];
	pid_t pid;
	int first_pipe[2];
	int second_pipe[2];
	int i;
    char command;
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

        pid=fork();

        if(pid==0)
        {
            read(first_pipe[READ_END], read_msg, BUFFER_SIZE);
		    printf("child read1 %s\n",read_msg);
            write(second_pipe[WRITE_END], "geldik   " , strlen(write_msg)+1);
        }
        else
        {
            cout << "Enter a command: ";   
            cin >> write_msg; 
            write(first_pipe[WRITE_END], write_msg, BUFFER_SIZE);
            if ( wait(0) == -1){
                perror("waiting\n");
            }

            read(second_pipe[READ_END], read_msg, BUFFER_SIZE);
            printf("parent read %s\n",read_msg);
        }
	}
	else {
        read(first_pipe[READ_END], read_msg, BUFFER_SIZE);
		printf("child read2 %s\n",read_msg);
        write(second_pipe[WRITE_END], "geldik   " , strlen(write_msg)+1);
        
	}

    if(pid==0)
    {
        read(first_pipe[READ_END], read_msg, BUFFER_SIZE);
		printf("child read1 %s\n",read_msg);
        write(second_pipe[WRITE_END], "geldik   " , strlen(write_msg)+1);

        close(second_pipe[READ_END]);
        close(second_pipe[WRITE_END]);
        exit(1);
    }
    else
    { 
    pid=fork();

    if(pid==0)
    {
        read(first_pipe[READ_END], read_msg, BUFFER_SIZE);
		printf("child read2 %s\n",read_msg);
        write(second_pipe[WRITE_END], "geldik   " , strlen(write_msg)+1);

        close(second_pipe[READ_END]);
        close(second_pipe[WRITE_END]);
        exit(1);
    }
    else
    {
        cout << "Enter a command: ";   
        cin >> write_msg; 
        write(first_pipe[WRITE_END], write_msg, BUFFER_SIZE);
        if ( wait(0) == -1){
            perror("waiting\n");
        }

        read(second_pipe[READ_END], read_msg, BUFFER_SIZE);
        printf("parent read %s\n",read_msg);

    }
}*/

    


