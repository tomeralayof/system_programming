/**********************************\
* Fixed-Size Allocator 		       *
* Developer: Tomer alayof      	   *
* Written : 0/01/2021              *
*                                  *
* Reviewer: Gal Tsaig  			   *
\**********************************/

#include <stdio.h> /*printf()*/
#include <assert.h> /* assert() */
#include <stdlib.h> /*malloc()*/
#include <string.h> /*strcpy()*/

#include "fsa.h"

size_t TestSuggestion();
fsa_t *TestInit(size_t suggestion);
void TestAllocAndFree(fsa_t * pool);


int main()
{	
	size_t suggest = TestSuggestion();
	fsa_t * fsa = TestInit(suggest);
	TestAllocAndFree(fsa);
	
	free(fsa);
	

	return 0;
}

size_t TestSuggestion()
{
	size_t num_of_blocks = 5;
	size_t block_size = 4;
	
	size_t suggestion = FSASuggestSize(num_of_blocks,block_size);
	
	assert(48 == suggestion);
	assert( suggestion == FSASuggestSize(num_of_blocks,8));	
	
	printf("SUGGESTION SUCCESS !\n");
	
	return suggestion;
}

fsa_t *TestInit(size_t suggestion)
{
	fsa_t *pool = NULL;
	size_t block_size = 4;
	
	pool = (fsa_t *)malloc(suggestion);
	pool = FSAInit(pool, suggestion, block_size); 
	
	printf("INIT SUCCESS!\n");
	
	return pool;
}

void TestAllocAndFree(fsa_t * pool)
{
	char *str1;
	char *str2;
	char *str3;
	char *str4;
	char *str5;
	char *str6;
	
	str1 = FSAAlloc(pool);
	assert(4 == FSACountFree(pool));
	
	str2 = FSAAlloc(pool);
	str3 = FSAAlloc(pool);
	str4 = FSAAlloc(pool);
	str5 = FSAAlloc(pool);
	
	assert(0 == FSACountFree(pool));
	str6 = FSAAlloc(pool);
	assert(0 == FSACountFree(pool));
	
	strcpy(str1, "tomer");
	strcpy(str2, "linoy");
	strcpy(str3, "ofir");
	strcpy(str4, "dan");
	strcpy(str5, "alona");
	
	assert(0 == strcmp(str1,"tomer"));
	assert(0 == strcmp(str2,"linoy"));
	assert(0 == strcmp(str3,"ofir"));
	assert(0 == strcmp(str4,"dan"));
	assert(0 == strcmp(str5,"alona"));
	
	FSAFree(pool, str1);
	FSAFree(pool, str2);
	FSAFree(pool, str3);
	FSAFree(pool, str4);
	FSAFree(pool, str5);

	assert(5 == FSACountFree(pool));
		
	str1 = FSAAlloc(pool);
	assert(4 == FSACountFree(pool));
	
	printf("ALLOC AND FREE SUCCESS! \n");
}





















