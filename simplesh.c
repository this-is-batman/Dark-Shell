#include<sys/stat.h>
#include<unistd.h>
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
    char* builtin_commands[] = {"dark_cd","dark_pwd","dark_env","dark_cl","dark_dir","help","exit"};
    int flag=0;
    int comm_len = sizeof(builtin_commands)/sizeof(char *);
    for(int i=0;i<comm_len;i++)
    {
        if(strcmp(args[0],builtin_commands[i])==0)
        {
            flag=1;
            break;
        }
    }
    if(flag==0)
    {
        printf("Command not found!\n");
        printf("Please type help for the list of available commands!\n");
        return 1;
    }
    if(strcmp(args[0],"help")==0)
    {
        printf( "Help mode of DARK_SHELL \n"
            "dark_cd: Change the current directory\n"
            "dark_pwd: Show the current working directory\n"
            "dark_env: List all the environment variables\n"
            "dark_cl: Clear the screen\n"
            "help: Show the list of available commands\n"
            "exit: Exit the DARK_SHELL\n"
        );
        return 1;
    }
    if(strcmp(args[0],"exit")==0){
     printf("Thanks for using DarkSHell! Hope to see you again soon!\n");
     exit(EXIT_SUCCESS);
    }
    if(strcmp(args[0],"dark_cd")==0)
    {
        if(args[1]==NULL)
        {
            fprintf(stderr, "dark_sh: Expected argument to dark_cd \n");
        }
        else
        {
           if(chdir(args[1]) !=0) perror("No such file or directory!\n"); 
        }
        return 1;
    }
    if(strcmp(args[0],"dark_pwd")==0)
    {
        if(args[1]!=NULL)
        {
            fprintf(stderr,"No argument is required for the command dark_pwd!\n");
        }
        else
        {
            char* buf=malloc(DARKSH_BUF*sizeof(char));
            if(getcwd(buf,DARKSH_BUF)=="NULL")
            {
                if(strcmp(strerror(errno),"ERANGE")==0)
                {
                    buf=realloc(buf,DARKSH_BUF*sizeof(char));
                    if(!buf) 
                    {
                        fprintf(stderr, "Reallocation failed!\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            else 
                printf("%s\n",getcwd(buf,DARKSH_BUF));
        }
        return 1;
    }
    if(strcmp(args[0],"dark_env")==0)
    {
        if(args[1]!=NULL)
        {
            fprintf(stderr, "No argument required for this!\n");
        }
        else
        {
            extern char** environ; //array of pointers to environment 
            int i=1;
            char *s= *environ;
            for(;s;i++)
            {
                printf("%s\n", s);
                s=*(environ+i); //printing all the elements in the array
            }
        }
        return 1;
    }
    if(strcmp(args[0],"dark_cl")==0)
    {
        const char* clear_screen = "\e[1;1H\e[2J";
        write(STDOUT_FILENO, clear_screen, 12);
        return 1;
    }
    if(strcmp(args[0],"dark_dir")==0) //create a new directory
    {
        if(args[1]==NULL)
        {
            fprintf(stderr, "Please enter the path of the directory\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            int status = mkdir(args[1], S_IRWXU);
            if(status==-1)
            {
                perror("Directory creation failed!\n");
            }
        }
    return 1;
    }
}
void dark_loop(void)
{
    char *line;
    char **args; //array containing the args
    int status;
    printf(" \t \t \t Welcome to DARK SHELL \n"
         "\t \t  This is built by ABHIRUP GUPTA CS1907 \n\n");
    do
    {
        printf("> ");
        char* buf = malloc(DARKSH_BUF*sizeof(char));
        if(getcwd(buf,DARKSH_BUF)=="NULL")
        {
            perror("Cannot read the current path!");
            exit(EXIT_FAILURE);
        }
        printf("%s ",getcwd(buf,DARKSH_BUF));
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