/**********************************\
* Fixed-Size Allocator 		       *
* Developer: Tomer alayof      	   *
* Written : 0/01/2021              *
*                                  *
* Reviewer: Gal Tsaig			   *
\**********************************/

#ifndef __FSA_H__
#define __FSA_H__

#include <unistd.h>
#include <sys/mman.h> 

typedef struct fsa fsa_t;

/*
* rational: 
	-tell the user how many bytes has to allocate for the fsa, the size will be aligned to word size.
*params:
	-size_t num_of_blocks: number of blockes the user want to create for allocating data.
	-size_t block_size : the size of each block 
*return:
	-number of bytes to allocate
*Complexity: 
	-time: O(1), space: O(1)
*/
size_t FSASuggestSize(size_t num_of_blocks, size_t block_size); 

/*
* rational: 
	-initialize the fsa .
*params:
	-void *memory: where the allocation start in the heap	
	-size_t mem_size: total size to be allocated
	-size_t block_size: size of block, the size will be aligned to be devided by word size.
*return:
	-pointer to fsa
*Complexity: 
	-time: O(n), space: O(1)
*/
fsa_t *FSAInit(void *memory, size_t mem_size, size_t block_size); 

/*
* rational: 
	-allocate memory to the blocks from the begining to the end 
*params:
	-fsa_t *pool: pointer to fsa
*return:
	-pointer to the block
*Complexity: 
	-time: O(1), space: O(1)
*/
void *FSAAlloc(fsa_t *pool); 

/*
* rational: 
	-release the block that for re-allocation. The function doesnt free the memory, free has to be called in the end of use. 
*params:
	-fsa_t *pool: pointer to fsa
	-void *block: poiner to the block to be released for re allocation.
*return:
	-no return
*Complexity: 
	-time: O(1), space: O(1)
*/
void FSAFree(fsa_t *pool, void *block); 

/*
* rational: 
	-count the number of blockes that are not in use. 
*params:
	-const fsa_t *pool: pointer to fsa.
*return:
	-number of un-used blockes
*Complexity: 
	-time: O(n), space: O(1)
*/
size_t FSACountFree(const fsa_t *pool); 

#endif /* __FSA_H__ */













