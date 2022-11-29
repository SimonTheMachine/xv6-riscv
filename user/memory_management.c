#include "memory_management.h"
#define NULL 0

// The actual head of the memory.
struct memoryBlock *headOfMemoryList = NULL;
// The current memory block.
struct memoryBlock *currentMemoryBlock = NULL;

// Function to allocate memory
void *_malloc(int size)
{

  // The size of the memory block is the size of the memoryBlock struct + the size of the memory requested
  int totalSize = size + sizeof(struct memoryBlock);

  // First we need to check if head is null. If it is, this means
  // we haven't allocated any memory yet. Then we need to initialize it.
  if (headOfMemoryList == NULL)
  {

    // Initializes pointer used to store the start of the memory grown by sbrk.
    char *startOfNewMemory;
    // System call that grows process memory by totalSize of bytes.
    startOfNewMemory = sbrk(totalSize);

    // If sbrk fails, we return 0
    if (startOfNewMemory == (char *)-1)
    {
      return 0;
    }
    // If sbrk succeeds, we set the head to the start of the new memory
    headOfMemoryList = (struct memoryBlock *)startOfNewMemory;
    // We set the size of the head to the size of the memory block
    headOfMemoryList->size = size;
    // We set the isFree flag to 0
    headOfMemoryList->isFree = 0;
    // We set the next pointer to null
    headOfMemoryList->next = NULL;
    // We set the current memory block to the head
    currentMemoryBlock = headOfMemoryList;
    // We return the address of the space after the memory block
    printf("Allocated %d bytes of memory \n", size);
    return (void *)(startOfNewMemory + sizeof(struct memoryBlock));
  }

  // When we iterate through the linked list of memory blocks we need to keep track of the previous block
  // Since we do next fit allocation, we need to be able to go back to head, and then continue until we
  // reach the current memory block again. This is why we have iterationStart.
  // We need to store tail so we can expand memory if we need to.
  struct memoryBlock *iterationStart, *tailOfMemoryList;
  iterationStart = currentMemoryBlock;
  tailOfMemoryList = NULL;

  // Here we check if there is a free block of memory that is big enough to hold the memory requested
  // We iterate until current is null, which means we have reached the end of the list
  do
  {

    if (currentMemoryBlock->isFree)
    {

      // If the current block is free and the next block is free, we merge the two blocks
      // We only need to check if the next block is free and not the one after it, because
      // of the way we do the free operation.
      if (currentMemoryBlock->next != NULL && currentMemoryBlock->next->isFree)
      {
        printf("Merging two free blocks of memory \n");
        currentMemoryBlock->size += currentMemoryBlock->next->size + sizeof(struct memoryBlock);
        currentMemoryBlock->next = currentMemoryBlock->next->next;
      }

      // If the current block is free and exact size we use it
      if (currentMemoryBlock->size == size)
      {
        printf("Using free block of memory of exact same size\n");
        // We set the isFree flag to 0
        currentMemoryBlock->isFree = 0;
        // We return the address of the space after the memory block
        return (void *)((char *)currentMemoryBlock + sizeof(struct memoryBlock));

        // If the current block is free and big enough for we split it.
        // Here we have to take into account that there needs to be space enough
        // For an additional memoryblock object as well.
      }
      else if ((currentMemoryBlock->size - sizeof(struct memoryBlock)) > size)
      {
        printf("Using free block of memory and splitting it\n");

        // First we create a new memory block after the current block
        struct memoryBlock *newBlock = (struct memoryBlock *)((char *)currentMemoryBlock + totalSize);
        // We set the size of the new block to the size of the current block minus the total size of the allocated memory block
        newBlock->size = currentMemoryBlock->size - totalSize;
        // We set the next pointer to the next pointer of the current block
        newBlock->next = currentMemoryBlock->next;
        // We set the isFree flag to 1 - free
        newBlock->isFree = 1;

        // now we adjust the current block:
        // We set the size of the current block to the size of the requested memory
        currentMemoryBlock->size = size;
        // We set the next pointer to the new block
        currentMemoryBlock->next = newBlock;
        // We set the isFree flag to 0
        currentMemoryBlock->isFree = 0;
        // We return the address of the space after the memory block
        return (void *)((char *)currentMemoryBlock + sizeof(struct memoryBlock));
      }
    }

    // If we reach the place we started iterating from then we need to allocate more memory eg. break the loop
    if (headOfMemoryList->next == NULL)
    {
      tailOfMemoryList = currentMemoryBlock;
      break;
    }
    // If the current block is not free or not big enough, we move on to the next block
    // If the next block is null, we set the current block to the head of the list
    if (currentMemoryBlock->next == NULL)
    {
      tailOfMemoryList = currentMemoryBlock;
      currentMemoryBlock = headOfMemoryList;
    }
    else
    {
      currentMemoryBlock = currentMemoryBlock->next;
    }
    if (currentMemoryBlock == iterationStart) {
      break;
    }
    
  } while (1);

  // If we get to this point, it means that there is no free block that is big enough
  // So we need to create a new block
  char *startOfNewMemory;
  // sbrk grows memory by the number of bytes specified
  startOfNewMemory = sbrk(totalSize);
  // If sbrk fails, we return 0
  if (startOfNewMemory == (char *)-1)
  {
    return 0;
  }
  printf("Allocated new bytes of memory because there wasn't enough space\n", size);  
  struct memoryBlock *newBlock;
  newBlock = (struct memoryBlock *)startOfNewMemory;
  // We set the size of the new block to the size of the memory block
  newBlock->size = size;
  // We set the isFree flag to 0
  newBlock->isFree = 0;
  // We set the next pointer to null
  newBlock->next = NULL;
  // If sbrk succeeds, we set the next pointer of the previous block to the start of the new memory
  tailOfMemoryList->next = newBlock;
  // We return the address of the space after the memory block
  return (void *)(startOfNewMemory + sizeof(struct memoryBlock));
}

// Function to free memory
void _free(void *ptr)
{
  // If the pointer is null, we do nothing
  if (ptr == NULL)
  {
    return;
  }
  // If the pointer is not null, we set the isFree flag of the memory block to 1
  struct memoryBlock *block = (struct memoryBlock *)((char *)ptr - sizeof(struct memoryBlock));
  block->isFree = 1;
  printf("Freed %d bytes of memory \n", block->size);

  // We also need to check if the next block is free. If it is, we merge the two blocks
  if (block->next && block->next->isFree)
  {
    // We have to take into account that merging current can be dangerous.
    if (block->next == currentMemoryBlock)
    {
      currentMemoryBlock = block;
    }
    block->size += block->next->size + sizeof(struct memoryBlock);
    block->next = block->next->next;
    printf("Merging two free blocks of memory inside free\n");
  }
  // In malloc we take into account that if the previous block is free, we merge the two blocks
}

int main(int argc, char *argv[])
{
  printf("Started main\n");
  // Checks basic malloc expansion
  // int* array =
  (int *)_malloc(100 * sizeof(int));
  printf("Allocated first\n");
  int *array2 = (int *)_malloc(50 * sizeof(int));
  printf("Allocated second\n");
  int *array3 = (int *)_malloc(50 * sizeof(int));
  printf("Allocated third\n");
  //int *array4 = 
  (int *)_malloc(100 * sizeof(int));
  printf("Done allocating\n");
  // Checks if free function works (with the memory merge thing)
  _free(array3);
  //
  int *array5 = (int *)_malloc(50 * sizeof(int));
  printf("should've done exact allocation\n");
  _free(array2);
  _free(array5);
  printf("should do merge memory in fron\n");
  array2 = (int *)_malloc(100 * sizeof(int));
  /*
  // This is to check if the future proof memory merge works
  _free(array4);
  printf("Done freeing\n");
  // Checks if it has merged them correctly
  
  array3 = (int *)_malloc(10 * sizeof(int));
  // Checks if it can allocate a new block even though some is free
  // int* array5 =
  (int *)_malloc(30 * sizeof(int));
  */

  printf("Done reallocating\n");
  exit(0);
}
