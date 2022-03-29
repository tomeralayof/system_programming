
#define _POSIX_SOURCE

#include <stdio.h>
#include <unistd.h> /*fork()*/
#include <signal.h> /*sigaction*/
#include <stdlib.h> /* fork */
#include <sys/types.h> /*pid_t*/
#include <string.h> /*strlen()*/

#define FAILURE (-1)

static void ChildProcess();
void SignalAction(int sig);

int main()
{
    struct sigaction sigaction1 = {0};

    sigaction1.sa_handler = &SignalAction;

    if (FAILURE == sigaction(SIGUSR2,&sigaction1,NULL))
    {
        return FAILURE;
    }

    ChildProcess();
    return 0;
}

static void ChildProcess()
{
    sleep(0);
    kill(getppid(),SIGUSR1);

    while (1)
    {
        sleep(0);
    }
}

void SignalAction(int sig)
{
    if (sig == SIGUSR2)
    {
        write(1,"pong\n",strlen("pong\n"));
        kill(getppid(),SIGUSR1);
    }
}