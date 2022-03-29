/*************************************\
* Variable-Size Allocator source code *
* Developer: Tomer Alayof             *
* Written:03/01/2022                  *
*                                     *
* Reviewer: Shoshana Huri             *
\*************************************/

#include <assert.h> /*assert*/

#include "vsa.h"

#define MAX(a, b) ((a) > (b)) ? (a) : (b)
#define MAGIC (0x4884102020100400L)


/*merge 2 following memory blocks that are not in use*/
static int VSADeFrag(vsa_t * pool ,char *block);

/*allocate block of memory inside the pool*/
static void * AllocMemory(char *pool_runner,size_t alloc_size);

/*word size alignment*/
static size_t Align(size_t alloc_size);

enum status
{
	FAILURE = -1,
	SUCCESS = 0
};

struct vsa  
{
	long size;
	#ifdef DEBUG
    long magic;
	#endif 
};

vsa_t *VSAInit(void *pool, size_t mem_size)
{
	vsa_t *vsa = NULL;	
	vsa_t *next = NULL;
	
	assert(NULL != pool);
	
	if(0 == mem_size)
	{
		return NULL;
	}
	vsa = pool;
	vsa->size = Align(mem_size);
	
	next = (vsa_t*)((char *)vsa + sizeof(vsa_t));
	next->size = vsa->size - sizeof(vsa_t);
	
	return vsa;
}

void *VSAAlloc(vsa_t *pool, size_t alloc_size)
{
	char *pool_runner = NULL;
	char * end = NULL;
	
	assert(NULL != pool);
	
	if(0 != alloc_size)
	{
		alloc_size = Align(alloc_size) + sizeof(vsa_t);
	
		pool_runner = (char *)pool + sizeof(vsa_t);
		end = (char *)pool + pool->size;
	
		while(pool_runner < end)
		{
			if ((long)alloc_size <= *(long *)pool_runner)
			{		
				return AllocMemory(pool_runner,alloc_size);
			}
			else if (0 > *(long *)pool_runner)
			{			
			pool_runner += (-1)*(*(long *)pool_runner);
			}
			else if ((long)alloc_size > *(long *)pool_runner)
			{
				if ( FAILURE == VSADeFrag(pool,pool_runner))
				{
					pool_runner += (*(long *)pool_runner);
				}
			}
		}
	}
		
	return NULL;
}

void VSAFree(void *block)
{
    if (NULL != block)
    {
    	block = (char *)block - sizeof(vsa_t);
     	
    	assert(0 > *(long *)block);
           
   		#ifdef DEBUG
        	assert(MAGIC == ((vsa_t *)block)->magic);
    	#endif

    	*(long *)block *= (-1);    
    }
}

size_t VSALargestFreeChunck(vsa_t *pool)
{
	long curr_max = 0;
	char *curr = NULL;
	char *end = NULL;
	
	assert(NULL != pool);
	
	curr = (char *)pool + sizeof(vsa_t);
	end = (char *)pool + pool->size;
	
	while(curr < end)
	{
		if (0 > *(long *)curr)
		{
			curr = curr + (-1) * *(long *)curr;
		}		
		else if (FAILURE == VSADeFrag(pool,curr))
		{
			curr_max = MAX(*(long *)curr, curr_max);
			curr += (*(long *)curr);	
		}	
	}
	
	return curr_max - sizeof(vsa_t);
}

static void * AllocMemory(char *pool_runner,size_t alloc_size)
{	
	void *ptr = NULL;
	
	long temp = *(long *)pool_runner;
	*(long *)pool_runner = -alloc_size;
	
	#ifdef DEBUG
	((vsa_t *)pool_runner)->magic = MAGIC;
	#endif

	ptr = (char *)pool_runner + sizeof(vsa_t);
	
	if (temp == (long)alloc_size)
	{
		return ptr;
	}
	
	pool_runner = (char *)pool_runner + alloc_size;
	*(long *)pool_runner = temp - alloc_size;
			
	return ptr;
}

static int VSADeFrag(vsa_t * pool ,char *block) 
{
	char *next = (char *)block;	
	char *end = (char *)pool + pool->size;

	next = block + *(long *)block;  
	
	if (next < end && *(long *)next > 0)
	{
		*(long*)block += *(long*)next;
		return SUCCESS;
	}

	return FAILURE;
}

static size_t Align(size_t alloc_size)
{	
	while ( alloc_size % sizeof(size_t) != 0)
	{
		++alloc_size;
	}
	
	return alloc_size;
}






