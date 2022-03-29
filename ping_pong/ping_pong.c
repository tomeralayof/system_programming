#define _POSIX_SOURCE

#include <unistd.h> /*fork()*/
#include <signal.h> /*sigaction*/
#include <stdlib.h> /* fork */
#include <sys/types.h> /*pid_t*/
#include <string.h> /*strlen()*/
#include <stdio.h> /*file*/

#include "../include/ping_pong.h"

#define PATH "./pong/a.out"
#define PONG_PATH "./pong_ex_4/pid.txt"
#define TRUE (1)

static void ParentProcess(pid_t child_pid);
static void ChildProcess();
static void SignalAction(int sig);
static void SignalHandlerEx4(int sig);
static status_t ReadPongPid(char buffer[60]);
static char buffer[60] = {0};
static pid_t pid = 0;

status_t PingEx4()
{
    struct sigaction sigaction1 = {0};

    sigaction1.sa_handler = &SignalHandlerEx4;

    if (FAILURE == sigaction(SIGUSR1,&sigaction1,NULL))
    {
        return FAILURE;
    }

    ParentProcess(pid);

    return SUCCESS;
}

static status_t ReadPongPid(char buffer[60])
{

    FILE *fp;
    status_t status = FAILURE;

    fp = fopen(PONG_PATH, "r");

    if (NULL != fp) 
    {         
        fgets(buffer, 60, fp);
        buffer[strlen(buffer)] = '\0';
        status = SUCCESS;
    }

    fclose(fp);

    return status;
}

status_t Ping()
{
    char *argv[] = {PATH,"a.out",NULL};
    struct sigaction sigaction1 = {0};
    sigaction1.sa_handler = &SignalAction;
    
    if (FAILURE == sigaction(SIGUSR1,&sigaction1,NULL))
    {
        return FAILURE;
    }

    pid = fork();
    if (FAILURE == pid)
    {
        return FAILURE;
    }
    if (0 == pid)
    {
        if (FAILURE == execvp(argv[0],argv))
        {
            return FAILURE;
        }
    }
    else
    {
        ParentProcess(pid);
    }

    return SUCCESS;
}

status_t PingPong()
{ 
    struct sigaction sigaction1 = {0};

    sigaction1.sa_handler = &SignalAction;

    if (FAILURE == sigaction(SIGUSR1,&sigaction1,NULL))
    {
        return FAILURE;
    }

    if (FAILURE == sigaction(SIGUSR2,&sigaction1,NULL))
    {
        return FAILURE;
    }

    pid = fork();

    if (FAILURE == pid)
    {
        return FAILURE;
    }

    if (0 == pid)
    {
        ChildProcess();
    }

    else
    {
        ParentProcess(pid); 
    }

    return SUCCESS;
}

void ChildProcess()
{
    kill(getppid(),SIGUSR1);
    pause();

    while (TRUE)
    {
        sleep(0);
    }
}

static void ParentProcess(pid_t child_pid)
{
    while (TRUE)
    {
        sleep(0);
    }
    
    kill(child_pid,SIGKILL);
}

static void SignalAction(int sig)
{
    if (SIGUSR2 == sig)
    {
        write(1,"pong\n",strlen("pong\n"));
        kill(getppid(),SIGUSR1);
    }
    else
    {
        write(1,"ping-",strlen("ping-"));
        kill(pid,SIGUSR2);
    }
}

static void SignalHandlerEx4(int sig)
{
    if(FAILURE == ReadPongPid(buffer))
    {
        return;
    }

    pid = atoi(buffer);

    if (sig == SIGUSR1)
    {
        write(1,"ping-",strlen("ping-"));
        kill(pid,SIGUSR2);
    }
}