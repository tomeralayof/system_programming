#include <fcntl.h>          /* For O_* constants */
#include <sys/stat.h>       /* For mode constants */
#include <semaphore.h>      /* sem_t */
#include <stdlib.h>         /* NULL */
#include <assert.h>         /* assert */
#include <stdio.h>          /* printf */

#include "../include/ilrd_semaphore.h"

#define SUCCESS (0)
#define PRIV_FLAG (0666)

sem_t *PosSemCreate(const char *sem_name, unsigned int initial_value)
{
    return sem_open(sem_name, O_CREAT, PRIV_FLAG, initial_value);
}

int PosSemWait(sem_t *sem, unsigned int number)
{
    int status = SUCCESS;
    size_t i = 0;

    for ( i = 0; i < number && (SUCCESS == status); ++i)
    {
        status = sem_wait(sem);
    }
    
    return status;
}

int PosSemPost(sem_t *sem, unsigned int number)
{
    int status = SUCCESS;
    size_t i = 0;

    for ( i = 0; i < number && (SUCCESS == status); ++i)
    {
        status = sem_post(sem);
    }

    return status;
}

int PosSemView(sem_t *sem)
{
    int value = 0;
    int status = SUCCESS;

    if(SUCCESS == (status = sem_getvalue(sem, &value)))
    {
        printf("sem value: %d\n", value);
    }

    return value;
}

int PosSemExit(sem_t *sem)
{
    return (SUCCESS != sem_close(sem));
}

int PosSemRemove(const char *sem_name)
{
    return (SUCCESS != sem_unlink(sem_name));
}








/* #include <stdio.h> 
#include <semaphore.h> 
#include <fcntl.h> 
#include <sys/stat.h>
#include <string.h> 
#include <assert.h> 

#include "../include/ilrd_semaphore.h"


sem_t *CreateSemaphore(const char *name,int number)
{
    sem_t *sem = sem_open(name, O_CREAT, 0600,number);
    if (SEM_FAILED == sem)
    {
        return NULL;
    }
    
    return sem;
}

status_t incrementSemaohore(sem_t *sem,int number)
{
    int i = 0;
    status_t status = SUCCESS;

    assert(number >= 0);

    for (i = 0; i < number && status == SUCCESS; i++)
    {
        status = sem_post(sem);
    }
    
    return status;
}

status_t DecrementSemaphore(sem_t *sem,int number)
{
    int i = 0;
    int sem_val = 0;
    status_t status = SUCCESS;

    assert(number >= 0);
    
    for (i = 0; i < number && status == SUCCESS; i++)
    {
        status = sem_wait(sem);
    }
    
    return status;
}

status_t GetSemaphoreVal(sem_t *sem)
{
    int sem_val = 0;

    if (FAILURE == sem_getvalue(sem, &sem_val))
    {
        return FAILURE;
    }

    printf("the sem val is: %d\n",sem_val);

    return SUCCESS;
} */