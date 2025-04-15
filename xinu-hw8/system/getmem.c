/**
 * @file getmem.c
 *
 * Modified by:	David Mathu, Luca Hergan
 *
 * TA-BOT:MAILTO david.mathu@marquette.edu
 * TA-BOT:MAILTO luca.hergan@marquette.edu
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
	 
	curr = head->head;
	prev = NULL;
	
	while (1) {
		if (curr->length > nbytes) {
			//we are going to take a chunk of memory out of looking_at
			//specifically from the start
			void* pointer_to_return = (void*) curr;
			struct memblock* new_block = (struct memblock*)(((void*)curr) + nbytes);
			new_block->length = curr->length - nbytes;
			new_block->next = curr->next;
			if (prev) {
				prev->next = new_block;
			} else {
				head->head = new_block;
			}
			head->length -= nbytes;
			return pointer_to_return;
		} else if (curr->length == nbytes) {
			//remove looking_at and return its address
			if (prev != NULL) {
				prev->next = curr->next;
			} else {
				head->head = curr->next;
			}
			curr->length = 0;
			curr->next = NULL;
			head->length -= nbytes;
			return (void*) curr;
		} else if (curr->next == NULL && curr->length < nbytes) {
			//curr is last block, we need to incheap and make remainder
			void* new_allocated_block = user_incheap(nbytes);
			if (nbytes % PAGE_SIZE == 0) {
				//there is no leftover block
				leftover = NULL;
			} else {
				// Prevent leftover from not having space for accounting memblock
				if (nbytes % PAGE_SIZE < sizeof(struct memblock)) user_incheap(PAGE_SIZE);
				leftover = (struct memblock*)(new_allocated_block + nbytes);
			}
			curr->next = leftover;
			if (leftover) { //setup leftover if it exists
				leftover->next = NULL;
				leftover->length = PAGE_SIZE - (nbytes % PAGE_SIZE);
			}
			// update head length
			head->length += PAGE_SIZE - (nbytes % PAGE_SIZE);
			return new_allocated_block;
		} else {
			prev = curr;
			curr = curr->next;
		}
	}

    return (void *)SYSERR;
}
