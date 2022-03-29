/*************************************\
* Variable-Size Allocator tests code  *
* Developer: Tomer Alayof             *
* Written:03/01/2022                  *
*                                     *
* Reviewer: ?????????????             *
\*************************************/

#include <stdio.h> /*printf()*/
#include <stdlib.h> /*malloc()*/
#include <string.h> /*strcpy()*/
#include <assert.h> /*assert*/

#include "vsa.h"

void TestAlloc();
void TestVSALargestFreeChunck(void);
void TestEdge();

int main()
{	
	TestVSALargestFreeChunck();
	
	return 0;
}

void  TestAlloc()
{

	void *pool = malloc(1000);
	
	char *p1 = NULL;
	char *p2 = NULL;
	char *p3 = NULL;
	char *p4 = NULL;
	char *p5 = NULL;
	char *p6 = NULL;
	
	
	
	vsa_t *my_vsa = VSAInit(pool, 1000);
	
	p1 = VSAAlloc(my_vsa, 56);
	p2 = VSAAlloc(my_vsa, 56);
	p3 = VSAAlloc(my_vsa, 56);
	p4 = VSAAlloc(my_vsa, 56);
	p5 = VSAAlloc(my_vsa, 56);
	
	
	VSAFree(p1);
	VSAFree(p2);
	VSAFree(p3);
	
	p1 = VSAAlloc(my_vsa, 100);
	
	p4 = (void *)p4;
	p5 = (void *)p5;
	p6 = (void *)p6;
	
	free(pool);

	
}

void TestVSALargestFreeChunck(void)
{
	void *pool = malloc(1000);
	char *p1 = NULL;
	char *p2 = NULL;
	char *p3 = NULL;
	char *p4 = NULL;
	

	vsa_t *my_vsa = VSAInit(pool, 1000); /* 8 8-984 */

	p1 = VSAAlloc(my_vsa, 48); /* 8 8-(-48) 8-928 */
	p2 = VSAAlloc(my_vsa, 48); /* 8 8-(-48) 8-(-48) 8-872 */
	p3 = VSAAlloc(my_vsa, 48); 
	p4 = VSAAlloc(my_vsa, 48);

	assert(760 == VSALargestFreeChunck(my_vsa));

	VSAFree(p1);
	VSAFree(p2);
	VSAFree(p3);

	assert(760 == VSALargestFreeChunck(my_vsa));

	p4 = VSAAlloc(my_vsa, 300); 

	assert(448 == VSALargestFreeChunck(my_vsa));

	p1 = VSAAlloc(my_vsa, 2000);
	assert(NULL == p1); 

	p4 = (void *)p4;

	free(pool); pool = NULL;
}


void TestEdge()
{
    void *pool = malloc(64);
    void *block = NULL;
    vsa_t *my_vsa = VSAInit(pool, 64);
    assert(48 == VSALargestFreeChunck(my_vsa));
    block = VSAAlloc(pool,48);
    assert(NULL != block);

    free(my_vsa); my_vsa = NULL;
} 



















