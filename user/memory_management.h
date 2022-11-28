#include "kernel/types.h"
#include "user/user.h"

//Data structure for a block of memory. 
//This is inserted before each allocated block of memory
struct memoryBlock{
 //The size (in bytes) of the memory allocated in this memory block
 int size;
 //Boolean to tell if the memory is free or not. 1 = free, 0 = not free
 int isFree;
  //Pointer to the next memory block
 struct memoryBlock *next; 
};
//Pointer to the head of the memory list
extern struct memoryBlock *headOfMemoryList;
//Function to allocate memory
void* _malloc(int size);
//Function to free memory
void _free(void *ptr);
