#include <stdio.h> /*printf()*/
#include <semaphore.h> /*sem syscalls*/
#include <fcntl.h> /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <stdlib.h> /* atoi */
#include <string.h> /*strcmp()*/
#include <assert.h> /*assert()*/

#include "../../include/semaphore_posix.h"

#define MAX_ASCII (256)
#define MAX_WORDS (250)
#define CMD_LIMITS (4)
#define SPACE (" ")

typedef enum 
{
    FALSE = 0,
    TRUE = 1
}bool; 

typedef status_t (*semaphore_opr_t)(sem_t *sem,int number);
typedef bool (*cmd_validator)(int *number,char *res,const char **cmd);

static status_t PosixActions(sem_t *new_sem,const char **cmd);
static status_t ExecuteUsrRqust(char *word, sem_t *new_sem,const char **cmd);
static status_t CommandAction(int number, char ch,sem_t *new_sem);

/*lut actions*/
static status_t GetSemaphoreVal(sem_t *sem,int number);
static status_t incrementSemaohore(sem_t *sem,int number);
static status_t DecrementSemaphore(sem_t *sem,int number);
static status_t Invalid(sem_t *sem,int number);
static status_t Remove(sem_t *sem,int number);
static status_t Exit(sem_t *sem,int number);
static semaphore_opr_t operations_lut[MAX_ASCII] = {0};

/*lut cmd validator*/
static bool ParserFirst(int *number,char *res,const char **cmd);
static bool SecondParser(int *number,char *res,const char **cmd);
static bool ThirdParser(int *number,char *res,const char **cmd);
static cmd_validator validator[CMD_LIMITS] = {0};

static void InitOperLut();
static void InitValidator();
static void InitialAllLuts();

status_t PosixSemManipulation(int argc,const char **cmd)
{
    sem_t *new_sem = NULL;
    
    assert(0 != atoi(cmd[2]));
    assert(argc > 2);

    new_sem = sem_open(cmd[1], O_CREAT, 0600,atoi(cmd[2]));
    if (SEM_FAILED == new_sem)
    {
        return FAILURE;
    }   

    InitialAllLuts();

    return PosixActions(new_sem,cmd);
}

static status_t PosixActions(sem_t *new_sem,const char **cmd)
{
    status_t status = SUCCESS;
    char word[MAX_WORDS] = {0};

    while (status != EXIT && status != FAILURE)
    {
        printf("enter a command: ");
        fgets(word,MAX_WORDS,stdin);
        status = ExecuteUsrRqust(word,new_sem,cmd);
        if (status == CMD_ERR)
        {
            printf("invalid cmd has accured \n");
        }
    }    

   return status;
}

static status_t ExecuteUsrRqust(char *word, sem_t *new_sem,const char **cmd)
{
    int number = 0;
    char command = 0;
    int counter = 0;
    bool is_valid = TRUE;

    char *res = strtok(word,SPACE);
    
    while (NULL != res && is_valid && counter < CMD_LIMITS)
    {
        if (counter == 1)
        {
            command = *res;
        }

        is_valid = validator[counter](&number,res,cmd);
        ++counter;
        res = strtok(NULL,SPACE);
    }  

    return !is_valid ? CMD_ERR : CommandAction(number,command,new_sem);
}

static status_t CommandAction(int number, char ch,sem_t *new_sem)
{
    return operations_lut[(int)ch](new_sem,number);
}

static void InitialAllLuts()
{
    InitOperLut();
    InitValidator();
}

static void InitValidator()
{
    validator[0] = &ParserFirst;
    validator[1] = &SecondParser;
    validator[2] = &ThirdParser;
}

static void InitOperLut()
{
    int i  = 0;

    for (i = 0; i < MAX_ASCII; i++)
    {
        operations_lut[i] = &Invalid;  
    }
    
    operations_lut['D'] = &DecrementSemaphore;
    operations_lut['I'] = &incrementSemaohore;
    operations_lut['V'] = &GetSemaphoreVal;
    operations_lut['E'] = &Exit;
    operations_lut['R'] = &Remove;
}

static bool ParserFirst(int *number,char *res,const char **cmd)
{
    (void)number;

    return strcmp(res,cmd[1]) == 0;
}

static bool SecondParser(int *number,char *res,const char **cmd)
{
    (void)number;
    (void)cmd;

    return strlen(res) <= 2; 
}

static bool ThirdParser(int *number,char *res,const char **cmd)
{
    (void)res;
    (void)cmd; 
    *number = atoi(res);

    return number != 0;
}

static status_t Invalid(sem_t *sem,int number)
{
    (void)sem;
    (void)number;

    return CMD_ERR;
}

static status_t GetSemaphoreVal(sem_t *sem,int number)
{
    int sem_val = 0;

    (void)number;

    if (FAILURE == sem_getvalue(sem, &sem_val))
    {
        return FAILURE;
    }

    printf("the sem val is: %d\n",sem_val);

    return SUCCESS;
}

static status_t incrementSemaohore(sem_t *sem,int number)
{
    int i = 0;
    status_t status = SUCCESS;

    for (i = 0; i < number && status == SUCCESS; i++)
    {
        status = sem_post(sem);
    }
    
    return status;
}

static status_t Exit(sem_t *sem,int number)
{
    (void)sem;
    (void)number;

    return EXIT;
}

static status_t Remove(sem_t *sem,int number)
{
    (void)number;

    if (FAILURE == sem_close(sem))
	{
		printf("falied to remove semaphore\n");
	}

    return EXIT;
}

static status_t DecrementSemaphore(sem_t *sem,int number)
{
    int i = 0;
    status_t status = SUCCESS;

    for (i = 0; i < number && status == SUCCESS; i++)
    {
        status = sem_wait(sem);
    }
    
    return status;
}