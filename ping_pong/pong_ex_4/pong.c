#define _POSIX_SOURCE

#include <unistd.h> /*getpid()*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h> /*sigaction*/
#include <assert.h>

static int pingId = 0;

typedef enum status
{
    FAILURE = 1,
    SUCCESS = 0
    
}status_t;

status_t Pong(int argc,char **argv);

static status_t WritePidToFile();
static void SignalHandlerEx4(int sig);
static void ChildProcess();

int main(int argc, char*argv[])
{
    Pong(argc,argv);

    return 0;
}

status_t Pong(int argc,char **argv)
{

    struct sigaction sigaction1 = {0};

    assert(argc > 1);

    pingId = atoi(argv[1]);

    if(FAILURE == WritePidToFile())
    {
        return FAILURE;
    }

    sigaction1.sa_handler = &SignalHandlerEx4;

    if (FAILURE == sigaction(SIGUSR2,&sigaction1,NULL))
    {
        return FAILURE;
    }

    ChildProcess();

    return SUCCESS;
}

static void ChildProcess()
{
    kill(pingId,SIGUSR1);
    pause();

    while (1)
    {
        pause();
        sleep(0);
    }
}

static void SignalHandlerEx4(int sig)
{
    if (sig == SIGUSR2)
    {
        write(1,"pong",strlen("pong"));
        kill(pingId,SIGUSR1);
    }
}

static status_t WritePidToFile()
{
    FILE *fptr;
    char buffer[100] = {0};
    int pid = getpid();
    int status = FAILURE;

    sprintf(buffer, "%d", pid);

    fptr = fopen("pid.txt", "w");

    if (fptr != NULL)
    {
        fputs(buffer,fptr);
        status = SUCCESS;
    }

    fclose(fptr);

    return status;
}