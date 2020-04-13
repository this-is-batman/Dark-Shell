#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
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

}

void dark_loop(void)
{
    char *line;
    char **args; //array containing the args
    int status;
    do
    {
        printf(">");
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