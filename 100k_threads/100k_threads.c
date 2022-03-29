
#include <pthread.h> /*pthread_create()*/
#include <stdio.h> /*printf()*/
#include <unistd.h> /*sleep*/
#include <time.h> /*time()*/
 #include <ulimit.h> /*ulimit()*/
 #include <omp.h> /* pragma omp parallel for num_threads */

typedef enum status
{
    FAILURE = -1,
    SUCCESS = 0
}status_t; 


#define MAX_ARR (100000)
#define MAX_POSSIBLE (32000)
#define OPT_THRDS_NUM (8)

static int global_arr[MAX_ARR] = {0};
static int global_arr2[MAX_ARR] = {0};
static int global_arr3[MAX_ARR] = {0};

status_t CreateThreads();
status_t CreateThreadsEx2();
status_t CreateThreadsEx3();
status_t CreateThreadsEx4();
void CreateThreadsEx5();

static time_t CreateThreadsAndMeasureTime4();
static time_t CreateThreadsAndMeasureTime();
static time_t CreateThreadsAndMeasureTime3();
static time_t CompareSingleMultiTimes();
void CreateThreadsEx6();

static void *InitArr(void *vargp);
static void *InitArr2(void *vargp);
static void *InitArr3(void *vargp);
static void *InitTHEx4(void *number);

static size_t user_num = 3200000000;
static size_t delta = 0;
    /*CreateThreadsEx2();*/

int main()
{
    
    int i = 0;
    printf("---ex.1---\n");
    printf("time in seconds using trheads = %ld\n",CreateThreadsAndMeasureTime());
    
    printf("---ex.3---\n");
    printf("time in seconds using detach thread ex.3 = %ld\n",CreateThreadsAndMeasureTime3());

    printf("---ex.4---\n");
    
    printf("time in seconds using single thread ex.4 = %ld\n",CreateThreadsAndMeasureTime4());
    
    printf("---ex.5---\n");
    printf("single - multi (second) = %ld\n",CompareSingleMultiTimes());

    printf("---ex.6---\n");
    CreateThreadsEx6();

    return 0;
}

static time_t CreateThreadsAndMeasureTime4()
{
    time_t begin, end,elapsed;
    time(&begin);

    printf("status = %d\n",CreateThreadsEx4());
    time(&end);

    elapsed = end - begin;

    return elapsed;
}

static time_t CompareSingleMultiTimes()
{
    time_t begin1, end1,elapsed1;
    time_t begin2, end2,elapsed2;

    time(&begin1);
    CreateThreadsEx4();
    time(&end1);
    elapsed1 = end1 - begin1;

    time(&begin2);
    CreateThreadsEx5();
    time(&end2);
    elapsed2 = end2 - begin2;

    return elapsed2 - elapsed1;
}


static time_t CreateThreadsAndMeasureTime()
{
    time_t begin, end,elapsed;
    time(&begin);

    printf("status = %d\n",CreateThreads());
    time(&end);

    elapsed = end - begin;

    return elapsed;
}

static time_t CreateThreadsAndMeasureTime3()
{
    time_t begin, end,elapsed;
    time(&begin);

    printf("status = %d\n",CreateThreadsEx3());
    time(&end);

    elapsed = end - begin;

    return elapsed;
}

status_t CreateThreads()
{
    unsigned long i = 0;
    unsigned long j = 0;
    pthread_t thread_id[MAX_ARR] = {0};
    status_t status = SUCCESS;

    for (i = 0; i < MAX_ARR && status == SUCCESS ; i++)
    {
        status = pthread_create(&thread_id[i],NULL,&InitArr,&i);
    }
    for (j = 0; j < i; ++j)
    {
        pthread_join(thread_id[j],NULL);
    }
    
    return status;
}

status_t CreateThreadsEx2()
{
    unsigned long i = 0;
    unsigned long j = 0;
    pthread_t thread_id[MAX_ARR] = {0};
    status_t status = SUCCESS;

    for (i = 0; i < MAX_ARR ; i++)
    {
        status = pthread_create(&thread_id[i],NULL,&InitArr2,&i);
        while (status != SUCCESS)
        {
            status = pthread_create(&thread_id[i],NULL,&InitArr2,&i);
        }
    }
    for (j = 0; j < i; ++j)
    {
        pthread_join(thread_id[j],NULL);
    }
    
    return status;
}


status_t CreateThreadsEx4()
{
    unsigned long i = 0;
    unsigned long j = 0;
    pthread_t thread_id[MAX_POSSIBLE] = {0};
    status_t status = SUCCESS;
    size_t curr_num = 1;
    size_t total_num_of_divisors = 0;
    delta = user_num / MAX_POSSIBLE;

    for (i = 0; i < MAX_POSSIBLE; ++i)
    {     
        status = pthread_create(&thread_id[i],NULL,&InitTHEx4,*(size_t **)&curr_num);
        while (status != SUCCESS)
        {
            status = pthread_create(&thread_id[i],NULL,&InitTHEx4,*(size_t **)&curr_num);
        }

        curr_num += delta;
    }

    for (j = 0; j < i; ++j)
    {
        size_t temp = 0;
        pthread_join(thread_id[j],(void*)&temp);
        total_num_of_divisors+=temp;
    }

    printf("result: %ld\n",total_num_of_divisors);
 
    return status;
}

static void *InitTHEx4(void *number)
{
    size_t i = 0;
    size_t sum_of_divisors = 0;
    size_t curr_num =  *(size_t *)&number;  

    for(i = curr_num;i < delta + curr_num; ++i)
    {
        if (0 == user_num % i)
        {
            sum_of_divisors += i;
        }
    }

    return (void *)sum_of_divisors;
}

status_t CreateThreadsEx3()
{
    unsigned long i = 0;
    unsigned long j = 0;
    pthread_t thread_id[MAX_ARR] = {0};
    status_t status = SUCCESS;

    for (i = 0; i < MAX_ARR; i++)
    {
        status = pthread_create(&thread_id[i],NULL,&InitArr3,&i);
        while (status != SUCCESS)
        {
            status = pthread_create(&thread_id[i],NULL,&InitArr3,&i);
        }
        pthread_detach(thread_id[i]);
    }
    
    return status;
}

static void *InitArr3(void *vargp)
{
    global_arr3[*(int *)vargp] = *(int *)vargp;
    return NULL;
}

static void *InitArr2(void *vargp)
{
    global_arr2[*(int *)vargp] = *(int *)vargp;
    return NULL;
}

static void *InitArr(void *vargp)
{
    global_arr[*(int *)vargp] = *(int *)vargp;
    return NULL;
}

void CreateThreadsEx5()
{
    size_t i = 0;
    size_t num = user_num;
    size_t num_of_divisors = 0;

    for (i = 1; i <= num; i++)
    {
        size_t sum_of_divisors = 0; 

        if (0 == num % i)
        {
            sum_of_divisors += i;
        }

        num_of_divisors+=sum_of_divisors;
    }

    printf("num = %ld\n",num_of_divisors);
}

void CreateThreadsEx6()
{
    size_t i = 0;
    size_t num = user_num;
    size_t num_of_divisors = 0;

    #pragma omp parallel for num_threads(OPT_THRDS_NUM)

    for (i = 1; i <= num; i++)
    {
        size_t sum_of_divisors = 0; 

        if (0 == num % i)
        {
            sum_of_divisors += i;
        }

        num_of_divisors+=sum_of_divisors;
    }

    printf("num = %ld\n",num_of_divisors);
}