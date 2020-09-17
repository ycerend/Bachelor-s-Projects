#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>

using namespace std;

#define BUFFER_SIZE 100
#define READ_END	0
#define WRITE_END	1
#define LSH_TOK_BUFSIZE 64

struct commandCharArray {
  char command[3];
};


struct CommandArguments {
    char** arguments;
    int size;
};

void free_CommandArguments(CommandArguments array)
{
    free(array.arguments);
}


int command_launch(char *first_pipe_args[],char *second_pipe_args[],char *third_pipe_args[]){
    
    int status;
    int i;
    // Executes the command "cat scores | grep Villanova | cut -b 1-10".
    // make 2 pipes (cat to grep and grep to cut); each has 2 fds

    int pipes[4];
    pipe(pipes); // sets up 1st pipe
    pipe(pipes + 2); // sets up 2nd pipe
    // we now have 4 fds:
    // pipes[0] = read end of first pipe (read by second)
    // pipes[1] = write end of second pipe (written by first)
    // pipes[2] = read end of second pipe (read by third)
    // pipes[3] = write end of third pipe (written by second)

    // Note that the code in each if is basically identical, so you
    // could set up a loop to handle it.  The differences are in the
    // indicies into pipes used for the dup2 system call
    // and that the 1st and last only deal with the end of one pipe.

    // fork the first child (to execute cat)
    if(first_pipe_args[0] != NULL){
        if (fork() == 0){
        // replace cat's stdout with write part of 1st pipe
                if(second_pipe_args[0]!= NULL)
                    dup2(pipes[1], 1);

                // close all pipes (very important!); end we're using was safely copied

                close(pipes[0]);
                close(pipes[1]);
                close(pipes[2]);
                close(pipes[3]);


                execvp(first_pipe_args[0], first_pipe_args);

                

        }else{
        // fork second child (to execute grep)
            if(second_pipe_args[0]!= NULL){
                if (fork() == 0){
                    if(first_pipe_args[0] != NULL)
                        dup2(pipes[0], 0);

                    // replace grep's stdout with write end of 2nd pipe
                    if(third_pipe_args[0] != NULL)
                        dup2(pipes[3], 1);

                // close all ends of pipes

                    close(pipes[0]);
                    close(pipes[1]);
                    close(pipes[2]);
                    close(pipes[3]);

                    execvp(second_pipe_args[0], second_pipe_args);
        // replace grep's stdin with read end of 1st pipe
                }else{
        // fork third child (to execute cut)
                    if(third_pipe_args[0] != NULL){
                        if (fork() == 0){
            // replace cut's stdin with input read of 2nd pipe
                            if(second_pipe_args[0] != NULL)
                                dup2(pipes[2], 0);
                            close(pipes[0]);
                            close(pipes[1]);
                            close(pipes[2]);
                            close(pipes[3]);
                            execvp(third_pipe_args[0], third_pipe_args);



            // close all ends of pipes
                        }
                    }
                }
            }

        }        
    // only the parent gets here and waits for 3 children to finish

        close(pipes[0]);
        close(pipes[1]);
        close(pipes[2]);
        close(pipes[3]);

        for (i = 0; i < 3; i++)
            wait(&status);
    }
    return 1;
}
    /*
    int status;
    int i;

	int first_pipe[2];

    if (pipe(first_pipe) == -1) {
        fprintf(stderr,"First Pipe failed");
        return 1;
    }

    int pid = fork();

    if (pid == 0)
    {
        // child gets here and handles "grep Villanova"

        // replace standard input with input part of pipe

        dup2(first_pipe[READ_END], READ_END);

        // close unused hald of pipe

        close(first_pipe[WRITE_END]);

        // execute grep
        if(!second_pipe_args[0])
            execvp(second_pipe_args[0], second_pipe_args);
    }
    else
    {
        if (fork() == 0)
	    {
        // parent gets here and handles "cat scores"

        // replace standard output with output part of pipe

        dup2(first_pipe[WRITE_END], WRITE_END);

        // close unused unput half of pipe

        close(first_pipe[READ_END]);

        // execute cat
        if(!first_pipe_args[0])
            execvp(first_pipe_args[0], first_pipe_args);
	    }

    }
    close(first_pipe[WRITE_END]);
    close(first_pipe[READ_END]);


    for (i = 0; i < 1; i++)
        wait(&status);

    return 1;*/

int execute_command(CommandArguments commandArgs)
{
    int i;
    if (!commandArgs.arguments[0]) {
    // An empty command was entered.
        return 1;
    }
    if(strcmp(commandArgs.arguments[0], "exit") == 0){
        cout<< "eli boş gelmiş" << endl;
        return 0;
    }
    
    /*for(int i = 0; i < commandArgs.size ; i++){
        if(strcmp(commandArgs.arguments[i], "listdir") == 0){

        }


    }*/
    vector<commandCharArray> deneme;
    commandCharArray cc ;

    char first_args[] = {"whoami", "sjsjsj", "nullptr"};
    char *second_args[] = {"grep",".c",nullptr};
    char *third_args[] = {nullptr,nullptr,nullptr};
    cc.command = first_args;

    deneme.push_back(cc);
    cout<< deneme[0].command[2]<<endl;
    return command_launch( first_args,third_args,third_args);
}





CommandArguments shell_split_line(char line[]){
    
    /*while (token != NULL) 
    { 
        printf("%s\n", token); 
        tokens.push_back(token);
        token = strtok(NULL, " "); 
        
    } */
    CommandArguments args;
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = (char**)malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
    }

    token = strtok(line, " ");
    while (token != NULL) {
        tokens[position] = token;
        position++;
        cout<< token << endl;

        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = (char**)realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
            fprintf(stderr, "lsh: allocation error\n");
            exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " ");
    }
    
    tokens[position] = NULL;
    args.arguments = tokens;
    args.size = position ;
    return args;

}


string shell_read_line()
{
    string line = "";
    getline(cin, line);
    return line;
}

void shell_loop(){
    string line = "";
    CommandArguments arguments;
    int status;
    string username =getenv("USER");

  do {
    cout <<username <<  " >>>";
    line = shell_read_line();
    //cout<< line << endl ;
    int n = line.length(); 
  
    // declaring character array 
    char char_array[n + 1]; 
  
    // copying the contents of the 
    // string to char array 
    strcpy(char_array, line.c_str()); 
    arguments = shell_split_line(char_array);
    status = execute_command(arguments);
    //free(line);
    free_CommandArguments(arguments);
  } while (status);
}



int main() {
    std::cout << "Hello, World!" << std::endl;
    
    

    shell_loop();
    return 0;
}