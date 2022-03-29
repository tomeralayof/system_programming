#include <semaphore.h> /*sem syscalls*/

typedef enum status
{
    FAILURE = -1,
    SUCCESS = 0
    
}status_t;

sem_t *PosSemCreate(const char *sem_name, unsigned int initial_value);
int PosSemPost(sem_t *sem, unsigned int number);
int PosSemView(sem_t *sem);
int PosSemExit(sem_t *sem);
int PosSemRemove(const char *sem_name);





/*
sem_t *CreateSemaphore(const char *name,unsigned int number);
int GetSemaphoreVal(sem_t *sem);
int incrementSemaohore(sem_t *sem,unsigned int number);
int DecrementSemaphore(sem_t *sem,int number);
int RemoveSemaphore(sem_t *sem,const char *name);
*/
/*int sem_unlink(const char *name);*/