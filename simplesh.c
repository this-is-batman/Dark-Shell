#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#define DARKSH_BUF 256
#define DARK_DELIM " \t\r\n\a"
char* dark_readline()
{
    char *line = NULL; //line is stored in this line buffer or string
    size_t bufsize=0;
    if(getline(&line,&bufsize,stdin) == -1)
    {
        if(feof(stdin))
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            perror("readline_error");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

char** dark_split_line(char* line)
{
    int bufsize = DARKSH_BUF, index=0;
    char **tokens=malloc(bufsize*sizeof(char*));
    char *token;
    if(!tokens)
    {
        fprintf(stderr, "Allocation failed");
        exit(EXIT_FAILURE);
    }
    token= strtok(line,DARK_DELIM);
    while(token!=NULL)
    {
        tokens[index] = token;
        index++;
        if(index >= bufsize) //allocate more memory
        {
            bufsize += DARKSH_BUF;
            tokens=realloc(tokens,bufsize*sizeof(char*));
            if(!tokens) //realloc failed
            {
                fprintf(stderr, "Reallocation failed! No space remaining!"); 
                exit(EXIT_FAILURE);
            }
        }
        token=strtok(NULL,DARK_DELIM); //go to the next token
    }
    tokens[index]=NULL; //end the array of char* with a NULL 
    return tokens;
}
int dark_execute(char** args)
{
    if(strcmp(args[0],"help")==0)
    {
        printf( "Help mode of DARK_SHELL \n"
            "dark_ls: list all the files in the current directory\n"
            "dark_cd: Change the current directory\n"
        );
    }
    if(strcmp(args[0],"exit")==0){
     printf("Thanks for using DarkSHell! Hope to see you again soon!\n");
     exit(EXIT_SUCCESS);
    }
}
void dark_loop(void)
{
    char *line;
    char **args; //array containing the args
    int status;
    do
    {
        printf(" \t \t \t Welcome to DARK SHELL \n"
         "\t \t  This is built by ABHIRUP GUPTA CS1907 \n");
        printf("> ");
        line = dark_readline();
        args = dark_split_line(line); // take the arguments from the line
        status = dark_execute(args); //here we will execute the command
        free(line);
        free(args);
    } while (status); //run in event loop while there is no problems
}
int main(int argc, char** argv)
{
    dark_loop(); //our shell loop will be present here
    return EXIT_SUCCESS;
}