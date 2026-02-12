#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>


/* Public Api : 

malloc _ 
free _ 

*/

void* myMALLOC(size_t size);

void myFREE(void* ptr );

#endif
