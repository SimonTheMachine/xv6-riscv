
#include "memory_management.h"

//Function to allocate memory
void* _malloc(int size) {
  struct memoryBlock *current, *previous;

  //First we need to check if head is null. If it is, we need to initialize it.
  if (head) {
    //If it is null, we need to initialize the head
    int totalSize = size + sizeof(struct memoryBlock);

    char *startOfNewMemory;
    //sbrk grows memory by the number of bytes specified
    startOfNewMemory = sbrk(totalSize);
    if (startOfNewMemory == (char*) -1) {
      //If sbrk fails, we return 0
      return 0;
    }
    //If sbrk succeeds, we set the head to the start of the new memory
    head = (struct memoryBlock*) startOfNewMemory;
    //We set the size of the head to the size of the memory block
    head->size = size;
    //We set the isFree flag to 0
    head->isFree = 0;
    //We return the address of the space after the memory block
    return (void*) (startOfNewMemory + sizeof(struct memoryBlock));    
  }

  //Next case: head is not null: we check if there is a free block that is big enough
  previous = head;
  current = head;
  while (current) {
    //If the current block is free and big enough, we use it
    if (current->isFree && current->size == size) {
      //We set the isFree flag to 0
      current->isFree = 0;
      //We return the address of the space after the memory block
      return (void*) ((char*) current + sizeof(struct memoryBlock));
    } else if (current->isFree && (current->size - sizeof(struct memoryBlock)) > size) {

      //First we create a new memory block after the current block
      struct memoryBlock *newBlock = (struct memoryBlock*) ((char*) current + sizeof(struct memoryBlock) + size);
      //We set the size of the new block to the size of the current block minus the size of the memory block and the size of the new block
      newBlock->size = current->size - size - sizeof(struct memoryBlock);
      //We set the next pointer to the next pointer of the current block
      newBlock->next = current->next;
      //We set the isFree flag to 1
      newBlock->isFree = 1;

      // now we adjust the current block:
      //We set the size of the current block to the size of the new block
      current->size = size;
      //We set the next pointer to the new block
      current->next = newBlock;
      //We set the isFree flag to 0
      current->isFree = 0;
      //We return the address of the space after the memory block
      return (void*) ((char*) current + sizeof(struct memoryBlock));
    }
    //If the current block is not free or not big enough, we move on to the next block
    previous = current;
    current = current->next;
  }
  //If we get to this point, it means that there is no free block that is big enough
  //So we need to create a new block
  int totalSize = size + sizeof(struct memoryBlock);
  char *startOfNewMemory;
  //sbrk grows memory by the number of bytes specified
  startOfNewMemory = sbrk(totalSize);
  if (startOfNewMemory == (char*) -1) {
    //If sbrk fails, we return 0
    return 0;
  }
  //If sbrk succeeds, we set the next pointer of the previous block to the start of the new memory
  previous->next = (struct memoryBlock*) startOfNewMemory;
  //We set the size of the new block to the size of the memory block
  previous->next->size = size;
  //We set the isFree flag to 0
  previous->next->isFree = 0;
  //We return the address of the space after the memory block
  return (void*) (startOfNewMemory + sizeof(struct memoryBlock));
}


//Function to free memory
void _free(void *ptr) {
  //If the pointer is null, we do nothing
  if (!ptr) {
    return;
  }
  //If the pointer is not null, we set the isFree flag to 1
  struct memoryBlock *block = (struct memoryBlock*) ((char*) ptr - sizeof(struct memoryBlock));
  block->isFree = 1;

  //We also need to check if the next block is free. If it is, we merge the two blocks
  if (block->next && block->next->isFree) {
    block->size += block->next->size + sizeof(struct memoryBlock);
    block->next = block->next->next;
  }
  //We also need to check if the previous block is free. If it is, we merge the two blocks
  struct memoryBlock *current = head;
  struct memoryBlock *previous = head;
  while (current) {
    if (current == block) {
      if (previous->isFree) {
        previous->size += current->size + sizeof(struct memoryBlock);
        previous->next = current->next;
      }
      return;
    }
    previous = current;
    current = current->next;
  }
}
