#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * @author xponch0x
*/

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define DELIMITERS " \t\r\n\a"

char** parse_command(char* line){
    int position = 0;
    char** tokens = malloc(MAX_ARGS * sizeof(char*));
    char* token;

    if(!tokens){
        fprintf(stderr, "[ALLOCATION ERROR]\n");
        exit(1);
    }

    token = strtok(line, DELIMITERS);
    while(token != NULL && position < MAX_ARGS - 1){
        tokens[position] = token;
        position++;
        token = strtok(NULL, DELIMITERS);
    }
    tokens[position] = NULL;
    return tokens;
}

int execute_command(char** args){
    pid_t pid;
    int status;

    if(args[0] == NULL){
        return 1;
    }

    if(strcmp(args[0], "exit") == 0){
        return 0;
    }

    if(strcmp(args[0], "cd") == 0){
        if(args[1] == NULL){
            fprintf(stderr, "[EXPECTED AN ARGUMENT FOR cd]\n");
        }
        else{
            if(chdir(args[1]) != 0){
                perror("shell");
            }
        }
        return 1;
    }
    
    pid = fork();
    if(pid == 0){
        if(execvp(args[0], args) == -1){
            perror("shell");
            exit(EXIT_FAILURE);
        }
    }
    else if(pid < 0){
        perror("shell");
    }
    else{
        do{
            waitpid(pid, &status, WUNTRACED);
        }while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int main(){
    char* line;
    char** args;
    int status = 1;
    char input[MAX_INPUT];

    while(status){
        printf("MODEL 666> ");

        if(!fgets(input, MAX_INPUT, stdin)){
            break;
        }

        args = parse_command(input);
        status = execute_command(args);

        free(args);
    }
    return EXIT_SUCCESS;
}