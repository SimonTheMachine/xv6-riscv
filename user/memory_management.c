#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

struct memoryChunk
{
  // Bool deciding if the chunk of memory is available or not. 0 means not free, 1 means free.
  int isFree;
  // How big this node is
  int size;
  // a struct to the next node:
  struct node *next;
};
// initialize the head of the linked list to nullpointer
struct memoryChunk *head = NULL;

// This method will allocate memory and assumes that there is space enough for the allocation
// This splits the first chunk (which is assumed to be free memory) into two chunks, one of which is the size of the allocation and the other is the rest of the memory
void splitAndInsert(struct memoryChunk *chunk, int size)
{
  chunk->size -= size;
  // If the size of the chunk is 0, we just allocate the new memory into the chunk
  if (chunk->size == 0)
  {
    chunk->isFree = 0;
    chunk->size = size;
    return;
  }
  // Here newChunk is the new chunk of memory that we will allocate
  struct memoryChunk *newChunk = NULL;
  newChunk->size = size;
  newChunk->isFree = 0;
  // We set the next pointer of the new chunk to the next pointer of the old chunk
  newChunk->next = chunk->next;
  // We set the next pointer of the old chunk to the new chunk
  chunk->next = newChunk;
  // Hereby we have inserted the new chunk and allocated memory for it to the right of the linked list.
}
void *_malloc(int size)
{
  // If the head is null, then we need to initialize the linked list
  // And allocate memory for it.
  if (head == NULL)
  {
    char *p;
    p = sbrk(size);
    head->isFree = 1;
    head->size = size;
    head->next = NULL;
    // return something IDK
    return p;
  }
  // If the head is free memory we know that it contains all the free memory. (this is because of the way
  // we have defined the free method). If the head is free memory and the size of the head is bigger than
  //  the size we want to allocate we split the head into the rest of free memory and an allocated memory.
  if (head->isFree == 1 && head->size >= size)
  {
    splitAndInsert(head, size);
    // return something IDK
    return head;
  }
  // If the head isn't free memory we need to expand the memory to make room for the new memory
  // we do this by expanding by size, adding the size to the head (the free memory location)
  // and then we split the free memory into the rest of the free memory and the allocated memory.
  char *p;
  p = sbrk(size);
  head->size += size;
  splitAndInsert(head, size);
}

void _free(void *ptr)
{
  // Free memory and merge with other free memory, so that all free memory is at the start of the linked list
  struct memoryChunk *iHead = head;
  // We traverse each node in the linked list
  while (iHead->next != NULL)
  {
    // Do some kind of comparison to see if the pointer is the same as the pointer in the linked list IDK
    if (iHead->isFree == 0 && iHead->next == ptr)
    {
      struct memoryChunk *freedMemoryChunk = iHead->next;
      // We then need to add the memory of the freed memory to the free memory chunk in the front of the linked list
      head->size += freedMemoryChunk->size;
      // We then need to remove the freed memory chunk from the linked list
      iHead->next = freedMemoryChunk->next;
      return;
    }
    iHead = iHead->next;
  }
}



/*
// This function will shuffle the non free memorychunks to the end of the linked list.
void shuffle()
{
  // We create a new head and a new tail
  struct memoryChunk *newHead = NULL;
  struct memoryChunk *newTail = NULL;

  // We create a temporary pointer to the head of the linked list
  struct memoryChunk *temp = head;

  // We loop through the linked list
  while (temp != NULL)
  {
    // If the chunk is not free we add it to the end of the linked list
    if (temp->isFree == 0)
    {
      // If the new head is nullpointer we set the new head to be the current chunk
      if (newHead == NULL)
      {
        newHead = temp;
        newTail = temp;
      }
      // If the new head is not nullpointer we set the next pointer of the new tail to be the current chunk and then set the new tail to be the current chunk
      else
      {
        newTail->next = temp;
        newTail = temp;
      }
    }
    // If the chunk is free we add it to the beginning of the linked list
    else
    {
      // If the new head is nullpointer we set the new head to be the current chunk
      if (newHead == NULL)
      {
        newHead = temp;
        newTail = temp;
      }
      // If the new head is not nullpointer we set the next pointer of the current chunk to be the new head and then set the new head to be the current chunk
      else
      {
        temp->next = newHead;
        newHead = temp;
      }
    }
    // We set the current chunk to be the next chunk in the linked list
    temp = temp->next;
  }
  // We also need to merge all the free memorychunks together
  temp = newHead;
  // We loop through the linked list
  while (temp != NULL)
  {
    // If the current chunk is free and the next chunk is free we merge them together
    if (temp->isFree == 1 && temp->next->isFree == 1)
    {
      // We set the size of the current chunk to be the size of the current chunk plus the size of the next chunk
      temp->size = temp->size + temp->next->size;
      // We set the next pointer of the current chunk to be the next pointer of the next chunk
      temp->next = temp->next->next;
    }
    // We set the current chunk to be the next chunk in the linked list
    temp = temp->next;
  }
  // We set the next pointer of the new tail to be nullpointer
  newTail->next = NULL;
  // We set the head of the linked list to be the new head
  head = newHead;
}

int tryAndInsertNewChunk(int size, struct memoryChunk *iHead)
{
  int found = 0;
  // We traverse each node in the linked list
  while (iHead->next != NULL && found == 0)
  {
    if (iHead->isFree == 1 && iHead->size >= size)
    {
      // then we need to perform a split where we allocate the memory
      // and then we need to create a new node for the rest of the memory
      split(iHead, size);
      found = 1;
    }
    else
    {
      iHead = iHead->next;
    }
  }
  return found;
}
*/
