/**********************************\
* Fixed-Size Allocator 		       *
* Developer: Tomer alayof      	   *
* Written : 0/01/2021              *
*                                  *
* Reviewer: Gal Tsaig  			   *
\**********************************/

#include <assert.h>
#include "fsa.h"

static size_t align(size_t num);

struct fsa
{
	long head;
};

static size_t align(size_t num)
{	
	if(num % sizeof(size_t) != 0)
	{
		num += sizeof(size_t) - (num % sizeof(size_t));
	}

	return num;
}

size_t FSASuggestSize(size_t num_of_blocks, size_t block_size)
{
	block_size = align(block_size);
	
	return sizeof(fsa_t) + (num_of_blocks * block_size);
}

fsa_t *FSAInit(void *memory, size_t mem_size, size_t block_size)
{
	fsa_t *fsa = NULL;
	char *list_runner = NULL; 
	char *end = NULL;
	
	assert(NULL != memory);

	block_size = align(block_size);
	fsa = memory;
	fsa->head = sizeof(fsa_t);	
	end = (char *)memory + mem_size - block_size;
	
	list_runner = memory;
	
	while (list_runner < end)
	{
		list_runner += *(long *)list_runner;
		*(long *) list_runner = block_size;
	}
	
	*(long *)list_runner = (long)memory - (long)list_runner;

	return memory;
}

void *FSAAlloc(fsa_t *pool)
{
 	void *ptr = NULL;
	
	assert(NULL != pool);
	
	if (0 != pool->head)
	{
		ptr = (char *)pool + pool->head;
		pool->head += *(long *)ptr;  
	}	
	
	return ptr;
}

void FSAFree(fsa_t *pool, void *block)
{
	assert(NULL != pool);
  	assert(NULL != block);

	*(long *)block = (long)pool + pool->head - (long)block;
	pool->head = (long)block - (long)pool; 	
}

size_t FSACountFree(const fsa_t *pool)
{
    size_t counter = 0;
    const char *list_runner = NULL;

    assert(NULL != pool);

    list_runner = (const char *)pool + pool->head;

    while((long)list_runner != (long)pool)
    {
        ++counter;
        list_runner += *(long *)list_runner;
    }

    return counter;
} 



