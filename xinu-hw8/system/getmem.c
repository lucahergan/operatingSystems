/**
 * @file getmem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Allocate heap memory.
 *
 *
 * @param nbytes
 *      Number of bytes requested.
 *
 * @return
 *      ::SYSERR if @p nbytes was 0 or there is no memory to satisfy the
 *      request; otherwise returns a pointer to the allocated memory region.
 *      The returned pointer is guaranteed to be 8-byte aligned.  Free the block
 *      with memfree() when done with it.
 */
void *getmem(ulong nbytes)
{
    register memblk *prev, *curr, *leftover;

    if (0 == nbytes)
    {
        return (void *)SYSERR;
    }

    /* round to multiple of memblock size   */
    nbytes = (ulong)roundmb(nbytes);
    struct memhead *head = (memhead *)PROCHEAPADDR;

    /* TODO:
     *      - Traverse through the freelist
     *        to find a block that's suitable 
     *        (Use First Fit with remainder splitting)
     *      - if there is no suitable block, call user_incheap
     *        with the request to add more pages to our process heap
     *      - return memory address if successful
     */
	 
	 struct memblock* previous = NULL;
	 struct memblock* looking_at = head->head;
	 
	 while (true) {
		 if (looking_at->length > nbytes) {
			 //split looking_at into allocation, and remainder
			 //"free block" will take up the first part, and the allocation will take the second part
			 //(so we don't have to modify pointers to the start of the free block!)
			 // ulong new_length = looking_at->length - nbytes;
			 looking_at->length -= nbytes;
			 return ((void*)looking_at) + (looking_at->length);
		 } else if (looking_at->length == nbytes) {
			 //remove looking_at and return its address
			 if (previous != NULL) {
				 previous->next = looking_at->next;
			 } else {
				 head->head = looking_at->next;
			 }
			 looking_at->length = 0;
			 looking_at->next = NULL;
			 return (void*) looking_at;
		 } else {
			 previous = looking_at;
			 looking_at = looking_at->next;
		 }
		 if (looking_at == NULL) {
			 //looking at end of list :( do the backup thing
			 return (void*) SYSERR;
		 }
	 }

    return (void *)SYSERR;
}
