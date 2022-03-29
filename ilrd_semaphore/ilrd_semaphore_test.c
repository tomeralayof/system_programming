#include "../include/ilrd_semaphore.h"

int main()
{
    sem_t *sem = PosSemCreate("avirewg",10);

    PosSemView(sem);

    PosSemPost(sem,20);

    PosSemView(sem);

    return 0;
}