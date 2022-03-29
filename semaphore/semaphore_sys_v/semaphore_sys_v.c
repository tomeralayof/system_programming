
#include <sys/sem.h> /* semget */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h> /* atoi */
#include <assert.h> /* assert */
#include <string.h> /*fgetc*/

#include "../../include/semaphore_sys_v.h"

status_t ExitProgram(int mutex_sem);
status_t RemoveSemaphore(int mutex_sem);

union semun {
    int val;                
    struct semid_ds *buf;   
    unsigned short *array;     
    struct seminfo *__buf;
}sem_attr;

status_t SysVSemManipulation(const char **cmd)
{
    key_t s_key;
    int mutex_sem;

    if ((s_key = ftok (cmd[1], 'S')) == FAILURE)
    {
        perror ("ftok");
        return FAILURE;
    }

    if ((mutex_sem = semget(s_key, 1, 0660 | IPC_CREAT)) == FAILURE) 
    {
        perror("semget");
        return FAILURE;
    }

    sem_attr.val = atoi(cmd[2]);
    if (semctl (mutex_sem, 0, SETVAL, sem_attr) == FAILURE)
    {
        perror ("semctl SETVAL");
        return FAILURE;
    }
}

status_t RemoveSemaphore(int mutex_sem)
{
    if (semctl (mutex_sem, 0, IPC_RMID) == FAILURE)
    {
        perror ("semctl IPC_RMID");
        return FAILURE;
    }

    return EXIT;
}


status_t DecrementSemaphore(int sem_id,int number)
{
    struct sembuf sem_opr = {0};
    
    sem_opr.sem_num = 0;
    sem_opr.sem_op = -number;

    if (semop (sem_id, &sem_opr, 1) == FAILURE) 
    {
        return FAILURE;
	    perror ("semop: spool_signal_sem");
    }

    return SUCCESS;
}

status_t IncrementSemapore(int sem_id,int number)
{
    struct sembuf sem_opr = {0};
    
    sem_opr.sem_num = 0;
    sem_opr.sem_op = number;

    if (semop (sem_id, &sem_opr, 1) == FAILURE) 
    {
        return FAILURE;
	    perror ("semop: spool_signal_sem");
    }

    return SUCCESS;
}

status_t SemaporeGetVal(int sem_id)
{
    int val = semctl(sem_id,0,GETVAL);
}



status_t ExitProgram(int mutex_sem)
{
    (void)mutex_sem;

    return EXIT;
}