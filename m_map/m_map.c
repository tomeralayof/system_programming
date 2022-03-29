#include <stdio.h> 	/* printf */
#include <stdlib.h>	/* malloc, free */

extern int extern_func(int num);
static int static_func(int var);

void const_static_local();
void non_const_local();
void const_local();
void const_local2();
void non_const_static_local();

int non_const_g1;
int non_const_g2 = 4;
const int CONST_G1 = 0;
const int CONST_G2 = 3;
const int CONST_G3 = 3;
const int CONST_G4 = 3;
static const int CONST_STATIC_G1 = 3;
static const int CONST_STATIC_G2 = 7;
int non_const_g3;
int non_const_g4 = 0;

static const int CONST_STATIC_G3 = 0;
static const int CONST_STATIC_G4 = 7;

static int non_const_static_g1;
static int non_const_static_g2 = 8;
static int non_const_static_g3 = 0;
static int non_const_static_g4 = 8;

extern int extern_func(int num)
{
	return num + 1;
}

static int static_func(int var)
{
	return var + 1;
}

int main(int argc, char *argv[], char *envp[])
{
	char *string_literal1 = "Hello World";
	char *string_literal2 = "";
	char *string_literal3 = "1";

	int *heap_variable = NULL;
	int *realloc_check = NULL;
	int *heap_variable2 = NULL;
	int *heap_variable3 = NULL;

	printf("-----------Code/Text----------------------\n");
	printf("extern func            : %p\n", extern_func);
	printf("static func            : %p\n", static_func);
	printf("\n");

	printf("------------RO Data-----------------------\n");
	printf("CONST_G1               : %p\n", (void *)&CONST_G1);
	printf("CONST_G2               : %p\n", (void *)&CONST_G2);
	printf("CONST_G3               : %p\n", (void *)&CONST_G3);
	printf("CONST_G4               : %p\n", (void *)&CONST_G4);

	printf("CONST_STATIC_G1        : %p\n", (void *)&CONST_STATIC_G1);
	printf("CONST_STATIC_G2        : %p\n", (void *)&CONST_STATIC_G2);
	printf("CONST_STATIC_G3        : %p\n", (void *)&CONST_STATIC_G3);
	printf("CONST_STATIC_G4        : %p\n", (void *)&CONST_STATIC_G4);

	printf("String1                : %p\n", (void *)string_literal1);
	printf("String2                : %p\n", (void *)string_literal2);
	printf("String3                : %p\n", (void *)string_literal3);

	const_static_local();

	printf("\n");
	
	printf("----------Data Segment--------------------\n");
	printf("non_const_g2           : %p\n", (void *)&non_const_g2);
	printf("non_const_static_g2    : %p\n", (void *)&non_const_static_g2);
	printf("non_const_static_g4    : %p\n", (void *)&non_const_static_g4);

	non_const_static_local();

	printf("\n");
	
	printf("-----------BSS Segment--------------------\n");
	printf("non_const_g1           : %p\n", (void *)&non_const_g1);
	printf("non_const_g3           : %p\n", (void *)&non_const_g3);
	printf("non_const_g4           : %p\n", (void *)&non_const_g4);
	printf("non_const_static_g1    : %p\n", (void *)&non_const_static_g1);
	printf("non_const_static_g3    : %p\n", (void *)&non_const_static_g3);
	
	printf("\n");

	printf("--------------Heap------------------------\n");
	heap_variable = (int *)malloc(1);
	heap_variable2 = (int *)malloc(2);
	heap_variable3 = (int *)malloc(3);

	realloc_check = realloc(heap_variable, 20);

	if (NULL == realloc_check)
	{
		return (1);
	}

	heap_variable = realloc_check;

	printf("Heap                   : %p\n", (void *)heap_variable);
	printf("Heap2                  : %p\n", (void *)heap_variable2);
	printf("Heap3                  : %p\n", (void *)heap_variable3);

	printf("\n");

	printf("--------------Stack-----------------------\n");
	non_const_local();


	printf("\n");

	printf("-------Environment/Command Line-----------\n");
	printf("EnvP                   : %p\n", (void *)&envp);
	printf("ArgV                   : %p\n", (void *)&argv);
	printf("ArgC                   : %p\n", (void *)&argc);
	printf("EnvP[0]                : %p\n", (void *)&envp[0]);
	printf("EnvP[1]                : %p\n", (void *)&envp[1]);
	
	free(heap_variable);
	free(heap_variable2);
	free(heap_variable3);

	return (0);

}

void const_static_local()
{
	static const int CONST_STATIC_LOCAL1 = 0;
	static const int CONST_STATIC_LOCAL2 = 5;
	static const int CONST_STATIC_LOCAL3 = 5;
	static const int CONST_STATIC_LOCAL4 = 5;

	printf("CONST_STATIC_LOCAL1    : %p\n", (void *)&CONST_STATIC_LOCAL1);
	printf("CONST_STATIC_LOCAL2    : %p\n", (void *)&CONST_STATIC_LOCAL2);
	printf("CONST_STATIC_LOCAL3    : %p\n", (void *)&CONST_STATIC_LOCAL3);
	printf("CONST_STATIC_LOCAL4    : %p\n", (void *)&CONST_STATIC_LOCAL4);
}

void non_const_local()
{
	int non_const_local1 = 2;
	int non_const_local2 = 2;
	int non_const_local3 = 2;
	int non_const_local4 = 2;

	const_local();

	printf("non_const_local1       : %p\n", (void *)&non_const_local1);
	printf("non_const_local2       : %p\n", (void *)&non_const_local2);
	printf("non_const_local3       : %p\n", (void *)&non_const_local3);
	printf("non_const_local4       : %p\n", (void *)&non_const_local4);

	non_const_local1 = static_func(non_const_local1);
}

void const_local()
{
	const int CONST_LOCAL1 = 1;
	const int CONST_LOCAL2 = 1;
	const int CONST_LOCAL3 = 1;
	const int CONST_LOCAL4 = 1;

	const_local2();

	printf("CONST_LOCAL1           : %p\n", (void *)&CONST_LOCAL1);
	printf("CONST_LOCAL2           : %p\n", (void *)&CONST_LOCAL2);
	printf("CONST_LOCAL3           : %p\n", (void *)&CONST_LOCAL3);
	printf("CONST_LOCAL4           : %p\n", (void *)&CONST_LOCAL4);
}

void const_local2()
{
	const int CONST_LOCAL21 = 1;
	const int CONST_LOCAL22 = 1;
	const int CONST_LOCAL23 = 0;
	const int CONST_LOCAL24 = 1;

	printf("CONST_LOCAL21          : %p\n", (void *)&CONST_LOCAL21);
	printf("CONST_LOCAL22          : %p\n", (void *)&CONST_LOCAL22);
	printf("CONST_LOCAL23          : %p\n", (void *)&CONST_LOCAL23);
	printf("CONST_LOCAL24          : %p\n", (void *)&CONST_LOCAL24);
}

void non_const_static_local()
{

	static int non_const_static_local1 = 4;
	static int non_const_static_local2 = 6;
	static int non_const_static_local3 = 4;
	static int non_const_static_local4 = 6;

	printf("non_const_static_local1: %p\n", (void *)&non_const_static_local1);
	printf("non_const_static_local2: %p\n", (void *)&non_const_static_local2);
	printf("non_const_static_local3: %p\n", (void *)&non_const_static_local3);
	printf("non_const_static_local4: %p\n", (void *)&non_const_static_local4);
}




