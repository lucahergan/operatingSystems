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
void *getmem(uint nbytes)
{
    register memblk *prev, *curr, *leftover;

    if (0 == nbytes)
    {
        return (void *)SYSERR;
    }

    /* round to multiple of memblock size   */
    nbytes = (uint)roundmb(nbytes);
    struct memhead *head = (memhead *)proctab[currpid].heaptop;

    /* TODO:
     *      - Traverse through the freelist
     *        to find a block that's suitable 
     *        (Use First Fit with remainder splitting)
     *      - if there is no suitable block, call user_incheap
     *        with the request to add more pages to our process heap
     *      - return memory address if successful
     */
	 
	 struct memblock* looking_at = head->head;
	 struct memblock* previous = NULL;
	 
	 while (1) {
		if (looking_at->length > nbytes) {
			//we are going to take a chunk of memory out of looking_at
			//specifically from the start
			void* pointer_to_return = (void*) looking_at;
			struct memblock* new_block = (struct memblock*)(((void*)looking_at) + nbytes);
			new_block->length = looking_at->length - nbytes;
			new_block->next = looking_at->next;
			if (previous) {
				previous->next = new_block;
			} else {
				head->head = new_block;
			}
			head->length -= nbytes;
			return pointer_to_return;
		} else if (looking_at->length == nbytes) {
			//remove looking_at and return its address
			if (previous != NULL) {
				previous->next = looking_at->next;
			} else {
				head->head = looking_at->next;
			}
			looking_at->length = 0;
			looking_at->next = NULL;
			head->length -= nbytes;
			return (void*) looking_at;
		} else {
			previous = looking_at;
			looking_at = looking_at->next;
		}
		if (looking_at == NULL) {
			//looking at end of list :( do the backup thing
			
			//TODO: call user_incheap
			// problem: incheap and heapinit are currently giving warnings of implicit declaration
			// so they're not   working
			
			return (void*) SYSERR;
		}
	}

    return (void *)SYSERR;
}
