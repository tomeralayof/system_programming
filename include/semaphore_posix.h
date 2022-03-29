#ifndef __SEMAPHORE_POSIX_H__
#define __SEMAPHORE_POSIX_H__

#include <limits.h>

typedef enum status
{
    FAILURE = -1,
    SUCCESS = 0,
	CMD_ERR = INT_MAX - 3,
	EXIT = INT_MAX - 2
}status_t;

status_t PosixSemManipulation(int argc,const char **cmd);

/*
	argv: "name size"

	decrement: "name D number (optional)u"  
	increment: "name I number (optional)u"
	view: name V
	exit: name X 
	remove : name R
 */

#endif /* __SEMAPHORE_POSIX_H__ */