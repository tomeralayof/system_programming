/**********************************\
* Var-Size Allocator Header        *
* Developer: Tomer Alayof          *
* Written: 2021-12-31              *
*                                  *
* Reviewer: Shoshana Huri          *
\**********************************/

#ifndef __VSA_H__
#define __VSA_H__

#include <stddef.h> /* size_t */

/**********************************************
* In C File:
* static void VSADeFrag(vsa_t *pool) 
* header struct for blocks - contains long offset to end of block (+ anything else needed?)
 **********************************************/
typedef struct vsa vsa_t;

/**
 * DESCRIPTION:
 * Initialize the value in memory.
 * Initing memory pools less than four words in size result in undefined behavior.
 * Complexity: O(1) time, O(1) space.
 * 
 * PARAMS:
 * pointer to the memory.
 * memory size if the mem_size isnt align we aligned to word size
 * 
 * 
 * RETURN:
 * pointer to the pool.
 * 
 */
vsa_t *VSAInit(void *pool, size_t mem_size);

/**
 * DESCRIPTION:
 * Allocate block for user use align to word size.
 * If the user goes beyond the boundaries of the block Undefine Behavior
 * Zero byte allocation requests result in undefined behavior.
 * Complexity: O(n) time, O(1) space.
 * 
 * PARAMS:
 * pointer to the pool. 
 * alloc_size 
 * 
 * RETURN:
 * In succsess return pointer to the pool.
 * If ther is no fit block return NULL. 
 */
void *VSAAlloc(vsa_t *pool, size_t alloc_size);

/**
 * DESCRIPTION:
 * Allocate block for user use.
 * free to NULL is define behavior
 * Freeing a previously freed block causes undefined behavior
 * Complexity: O(1) time, O(1) space.
 * 
 * PARAMS:
 * pointer to the block we want to free. 
 * 
 * 
 * RETURN:
 * None
 * 
 */
void VSAFree(void *block);

/**
 * DESCRIPTION:
 * the size of the bigest free chunck
 * Complexity: O(n) time, O(1) space.
 * 
 * PARAMS:
 * pointer to the pool. 
 * 
 * 
 * RETURN:
 * the size of the bigest free chunck
 * 
 */
size_t VSALargestFreeChunck(vsa_t *pool);

#endif /* __VSA_H__ */

