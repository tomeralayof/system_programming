
#include <stdio.h>/*printf*/
#include <pthread.h> /*threade create */
#include <semaphore.h>/*semaphore*/
#include <time.h> /*time*/

#include "../../ds/include/dll.h"
#include "../../ds/include/cir_buffer.h"

#include "../../ds/dll/dll.c"
#include "../../ds/cir_buffer/cir_buffer.c"

#define TRUE (1)
#define MAX_ARR (10)
#define CONSUMER (1)
#define SUCCESS (0)
#define NUM_OF_THREADS (10)

static int flag = 0;
static pthread_mutex_t count_mutex;
static pthread_mutex_t count_mutex2;
static sem_t sem;
static sem_t sem_write_ex_4;
static sem_t sem_read_ex_4;
static cbuffer_t *buf = NULL; 
static dlist_t *dll = NULL;
const int input[] = {1,2,3,4,5,6,7,8,9,10};
static int output[10] = {0};

/*ex 5*/
static cbuffer_t *buf_ex_5 = NULL;
static int output_ex_5[10] = {0};
static sem_t sem_write_ex_5;
static sem_t sem_read_ex_5;
static pthread_mutex_t mutex_write_ex_5;
static pthread_mutex_t mutex_read_ex_5;
/*ex 5*/

/*ex 6*/
static pthread_mutex_t mutex_ex_6;
static sem_t sem_ex_6;
static int cons = 0;
static pthread_cond_t cond1;
static int brod = 0;

/*ex 6*/

void Exercise1();
void Exercise2();
void Exercise3();
void Exercise4();
void Exercise5();
void Exercise6();

static void *ConsumerAction(void *vargp);
static void *ProducerAction(void *vargp);

static void *ProducerActionEx2(void *vargp);
static void *ConsumerActionEx2(void *vargp);

static void *ProducerActionEx3(void *vargp);
static void *ConsumerActionEx3(void *vargp);

static void *ProducerActionEx4(void *vargp);
static void *ConsumerActionEx4(void *vargp);

static void *ProducerActionEx5(void *vargp);
static void *ConsumerActionEx5(void *vargp);

static void *ProducerActionEx6(void *ptr);
static void *ConsumerActionEx6(void *ptr);
static void UpdateSemaphore();

int main()
{
    /*
    Exercise1();
    Exercise2();
    Exercise3();
    Exercise4();
    Exercise5();
    */
    Exercise6();



    return 0;
}

/*ex. 1*/
void Exercise1()
{
    dlist_t *dll = DListCreate();

    pthread_t thread_id_producer = 0;
    pthread_t thread_id_consumer = 0;
  
    pthread_create(&thread_id_producer, NULL, &ProducerAction, dll);
    pthread_create(&thread_id_consumer, NULL, &ConsumerAction, dll);

    pthread_join(thread_id_consumer,NULL);
    pthread_join(thread_id_producer,NULL);

    DListDestroy(dll);
}

/*BUSY WAITING, EVERY THREAD WAIT FOR ONOTHER TREAH TO RELEASE ..*/
static void *ConsumerAction(void *vargp)
{
    while (TRUE)
    {  
        if (CONSUMER == flag)
        {
            printf("data = %d\n",*(int *)DListGetData(DListBegin((dlist_t *)vargp)));
            DListPopBack((dlist_t *)vargp);
            flag = !CONSUMER;
        }
    }

    return NULL;
}

static void *ProducerAction(void *vargp)
{
    int counter = 0;

    while (TRUE)
    {
        if (CONSUMER != flag)
        {
            DListPushBack((dlist_t *)vargp, &counter);
            ++counter;
            flag = CONSUMER;
        }
    }

    return NULL;
}

/*ex 2.*/
void Exercise2()
{
    int i = 0;
    int status = 0;
    pthread_t thread_producer_id[MAX_ARR] = {0};
    pthread_t thread_consumer_id[MAX_ARR] = {0};

    dll = DListCreate();

    if (SUCCESS != pthread_mutex_init(&count_mutex2, NULL))
    {
        return;
    }

    for (i = 0; i < MAX_ARR && SUCCESS == status; i++)
    {
        status = pthread_create(&thread_producer_id[i],NULL,&ProducerActionEx2,NULL);
        status = pthread_create(&thread_consumer_id[i],NULL,&ConsumerActionEx2,NULL);
    }

    for (i = 0; i < MAX_ARR ; i++)
    {
        pthread_join(thread_producer_id[i],NULL);
        pthread_join(thread_consumer_id[i],NULL);
    }
    
    pthread_mutex_destroy(&count_mutex2);
}

/*CONSUMERS IN BUSY WAITING ..*/
static void *ProducerActionEx2(void *vargp)
{
    static int x = 0;
    pthread_mutex_lock(&count_mutex2);
    DListPushBack(dll,(void *)x);
    ++x;
    pthread_mutex_unlock(&count_mutex2);

    return NULL;
}

static void *ConsumerActionEx2(void *vargp)
{
    while (1)
    {
        pthread_mutex_lock(&count_mutex2);
        if (DListIsEmpty(dll))
        {
            pthread_mutex_unlock(&count_mutex2);
        }
        else
        {
            break;
        }        
    }

    printf("data = %d\n",(int)DListGetData(DListBegin(dll)));
    DListPopBack(dll);
    pthread_mutex_unlock(&count_mutex2);

    return NULL;
}

/*ex 3.*/
void Exercise3()
{
    pthread_t thread_producer_id[MAX_ARR] = {0};
    pthread_t thread_consumer_id[MAX_ARR] = {0};
    int i = 0;
    int status = 0;
    int j = 0;

    dlist_t *dll = DListCreate();

    if (SUCCESS != pthread_mutex_init(&count_mutex, NULL) || SUCCESS != sem_init(&sem,0,0))
    {
        return;
    }

    for (i = 0; i < MAX_ARR && SUCCESS == status; i++)
    {
        status = pthread_create(&thread_producer_id[i],NULL,&ProducerActionEx3,dll);
        status = pthread_create(&thread_consumer_id[i],NULL,&ConsumerActionEx3,dll);
    }

    for(j = 0; j < i; ++j)
    {
        pthread_join(thread_producer_id[j], NULL);
        pthread_join(thread_consumer_id[j], NULL);
    }

    pthread_mutex_destroy(&count_mutex);
    DListDestroy(dll);
    sem_destroy(&sem);
}
/*PREVENT THE BUSY WAITING*/
static void *ProducerActionEx3(void *vargp)
{
    static int x = 10;

    pthread_mutex_lock(&count_mutex);
    DListPushBack((dlist_t *)vargp, (void *)x);
    ++x;
    sem_post(&sem);
    pthread_mutex_unlock(&count_mutex); 
}

static void *ConsumerActionEx3(void *vargp)
{
    sem_wait(&sem);

    pthread_mutex_lock(&count_mutex);
    printf("data = %d\n",(int)DListGetData(DListBegin((dlist_t *)vargp)));
    DListPopBack((dlist_t *)vargp);
    pthread_mutex_unlock(&count_mutex);
}

/*----------ex 4.----------*/
void Exercise4()
{
    pthread_t thread_producer_id[MAX_ARR] = {0};
    pthread_t thread_consumer_id[MAX_ARR] = {0};
    int i = 0;
    int j = 0;
    int status = 0;

    pthread_mutex_init(&count_mutex, NULL);
    sem_init(&sem_write_ex_4,0,10);/*how many to write*/
    sem_init(&sem_read_ex_4,0,0);/*how many to read*/

    buf = CBuffCreate(10 * sizeof(int));

    for (i = 0; i < MAX_ARR && SUCCESS == status; i++)
    {
        status = pthread_create(&thread_producer_id[i],NULL,&ProducerActionEx4,NULL);
        status = pthread_create(&thread_consumer_id[i],NULL,&ConsumerActionEx4,NULL);
    }

    for(j = 0; j < i; ++j)
    {
        pthread_join(thread_producer_id[j], NULL);
        pthread_join(thread_consumer_id[j], NULL);
    }
    
    pthread_mutex_destroy(&count_mutex);

    sem_destroy(&sem_write_ex_4);
    sem_destroy(&sem_read_ex_4);
    CBuffDestroy(buf);
}

static void *ProducerActionEx4(void *vargp)
{ 
    static int i = 0;

    sem_wait(&sem_write_ex_4);
    pthread_mutex_lock(&count_mutex);
    CBuffWrite(buf,input + i,sizeof(int));
    ++i;
    sem_post(&sem_read_ex_4);
    pthread_mutex_unlock(&count_mutex);
}

static void *ConsumerActionEx4(void *vargp) 
{
    static int i = 0;

    sem_wait(&sem_read_ex_4);
    pthread_mutex_lock(&count_mutex);
    CBuffRead(buf,output + i,sizeof(int));
    printf("output = %d\n",output[i]);
    ++i;
    sem_post(&sem_write_ex_4);
    pthread_mutex_unlock(&count_mutex);
}

/*----------ex 5.----------*/
void Exercise5()
{
    pthread_t thread_producer_id[MAX_ARR] = {0};
    pthread_t thread_consumer_id[MAX_ARR] = {0};

    int i = 0;
    int j = 0;
    int status = 0;

    pthread_mutex_init(&mutex_write_ex_5, NULL);
    pthread_mutex_init(&mutex_read_ex_5, NULL);

    sem_init(&sem_write_ex_5,0,10);/*how many to write*/
    sem_init(&sem_read_ex_5,0,0);/*how many to read*/

    buf_ex_5 = CBuffCreate(10 * sizeof(int));

    for (i = 0; i < MAX_ARR && SUCCESS == status; i++)
    {
        status = pthread_create(&thread_producer_id[i],NULL,&ProducerActionEx5,NULL);
        status = pthread_create(&thread_consumer_id[i],NULL,&ConsumerActionEx5,NULL);
    }

    for(i = 0; i < MAX_ARR; ++i)
    {
        pthread_join(thread_producer_id[i], NULL);
        pthread_join(thread_consumer_id[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex_write_ex_5);
    pthread_mutex_destroy(&mutex_read_ex_5);

    sem_destroy(&sem_write_ex_5);
    sem_destroy(&sem_read_ex_5);

    CBuffDestroy(buf_ex_5);
}

static void *ProducerActionEx5(void *vargp)
{
    static int i = 0;

    sem_wait(&sem_write_ex_5);
    pthread_mutex_lock(&mutex_write_ex_5);
    CBuffWrite(buf_ex_5,input + i,sizeof(int));
    i  = (i + 1) % MAX_ARR;
    sem_post(&sem_read_ex_5);
    pthread_mutex_unlock(&mutex_write_ex_5);
}

static void *ConsumerActionEx5(void *vargp)
{
    static int i = 0;

    sem_wait(&sem_read_ex_5);
    pthread_mutex_lock(&mutex_read_ex_5);
    CBuffRead(buf_ex_5,output_ex_5 + i,sizeof(int));
    printf("output = %d\n",output_ex_5[i]);
    i  = (i + 1) % MAX_ARR;
    sem_post(&sem_write_ex_5);
    pthread_mutex_unlock(&mutex_read_ex_5);
}

/*-------------------ex6.----------------*/
void Exercise6()
{
    pthread_t thread_producer_id;
    pthread_t thread_consumer_id[NUM_OF_THREADS] = {0};

    int i = 0;
    int j = 0;
    int status = 0;

    pthread_mutex_init(&mutex_ex_6, NULL);
    sem_init(&sem_ex_6,0,NUM_OF_THREADS);
    pthread_cond_init(&cond1,NULL);

    if (SUCCESS !=  (status = pthread_create(&thread_producer_id,NULL,&ProducerActionEx6,NULL)))
    {
        return;
    }

    for (i = 0; i < NUM_OF_THREADS && SUCCESS == status; i++)
    {
        status = pthread_create(&thread_consumer_id[i],NULL,&ConsumerActionEx6,NULL);
    }

    pthread_join(thread_producer_id, NULL);

    for(i = 0; i < NUM_OF_THREADS; ++i)
    {
        pthread_join(thread_consumer_id[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex_ex_6);

    sem_destroy(&sem_ex_6);
}

static void *ProducerActionEx6(void *ptr)
{
    time_t start = time(NULL);

    while (time(NULL) < start + 0.2)
    {
        pthread_mutex_lock(&mutex_ex_6);
        while (cons != NUM_OF_THREADS)
        { 
            pthread_cond_wait(&cond1, &mutex_ex_6);
            sleep(0);
        }
       
        printf("sending massage...\n");
        cons = 0;
        UpdateSemaphore();
        pthread_mutex_unlock(&mutex_ex_6);
        
    }
}

static void *ConsumerActionEx6(void *ptr)
{
    time_t start = time(NULL);

    while (time(NULL) < start + 0.2)
    {   
        sem_wait(&sem_ex_6);

        pthread_mutex_lock(&mutex_ex_6);
        printf("receiving massage...%d\n",(int)pthread_self());
        ++cons;
        
        pthread_mutex_unlock(&mutex_ex_6);
        pthread_cond_signal(&cond1);
        sleep(0);
    }
}

static void UpdateSemaphore()
{
    int i = 0;
    for (i = 0; i < NUM_OF_THREADS; i++)
    {
        sem_post(&sem_ex_6);
    }
}