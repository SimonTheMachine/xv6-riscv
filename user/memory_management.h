#include "kernel/types.h"
//#include "kernel/stat.h"
#include "user/user.h"
//#include "kernel/param.h"

struct memoryBlock{
 int size;
 int isFree;
 struct memoryBlock *next; 
};

extern struct memoryBlock *headOfMemoryList;

void* _malloc(int size);
void _free(void *ptr);
