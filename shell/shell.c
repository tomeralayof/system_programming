#include <stdlib.h> /*system()*/
#include <unistd.h> /*fork()*/
#include <stdio.h> /*printf()*/
#include <string.h> /*strcmp()*/
#include <assert.h> /*assert()*/
#include <sys/wait.h> /*wait()*/

#include "../include/shell.h"

#define INPUT_SIZE (1024)

typedef enum status
{
    ERROR = -1,
    SUCCESS = 0
} status_t;

static status_t SystemState();
static status_t ForkState();
static int ExitPressed(char *word);
static int ForkPressed(char **argv);
static int SystemPressed(char ** argv);

int Shell(int argc,char **argv)
{ 
    assert(argc > 1);
    assert(ForkPressed(argv) || SystemPressed(argv));
    printf("enter command line using %s: ",argv[1]);

    return ForkPressed(argv) ? ForkState() : SystemState();
}

static status_t SystemState()
{
    char word[INPUT_SIZE] = {0};
    status_t status = SUCCESS;

    while(!ExitPressed(word) && ERROR != status)
    {
       status = system(word);
       printf("enter command line using system: ");
    }

    return status;
}

static status_t ForkState()
{
    char word[INPUT_SIZE] = {0};
    
    while (!ExitPressed(word))
    {  
        int pid = fork();

        if (ERROR == pid)
        {
            return ERROR;
        }

        if (0 == pid)
        {
            if (ERROR == execl("/bin/sh", "sh", "-c", word, NULL))
            {
                return ERROR;
            }      
        }
        else
        {
            printf("enter command line using fork: ");
            wait(NULL);
        }
    }
    
    return SUCCESS;
}

static int ExitPressed(char *word)
{
   return strcmp(fgets(word,1024,stdin), "exit\n") == 0;
}

static int ForkPressed(char **argv)
{
    return strcmp(argv[1],"fork") == 0;
}

static int SystemPressed(char **argv)
{
    return strcmp(argv[1],"system") == 0;
}