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
	 
	curr = head->head;
	prev = NULL;
	 
	 while (1) {
		if (curr->length > nbytes) {
			//we are going to take a chunk of memory out of looking_at
			//specifically from the start
			void* pointer_to_return = (void*) curr;
			// kprintf("pointer to return part 1: %08x \r\n", pointer_to_return);
			// kprintf("pointer to return part 1: %08x \r\n", pointer_to_return);
			struct memblock* new_block = (struct memblock*)(((void*)curr) + nbytes);
			new_block->length = curr->length - nbytes;
			new_block->next = curr->next;
			if (prev) {
				prev->next = new_block;
			} else {
				head->head = new_block;
			}
			head->length -= nbytes;
			// kprintf("pointer to return part 2: %08x \r\n", pointer_to_return);
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
		}
		
			//Recursive 
		
		/* else {
			prev = curr;
			curr = curr->next;
		}
		if (curr == NULL) {
			//looking at end of list :( do the backup thing
			//create new pages, and let the block previous take them on
			//take stuff out of this new giant block
			
			ulong new_space = roundpage(nbytes - prev->length);
			user_incheap(new_space);
			prev->length += new_space;
			head->length += new_space;
			return getmem(nbytes);
		}
		//Iterative
		else if (curr->next == NULL && curr->length < nbytes) {
			ulong new_space_needed;
			new_space_needed = roundpage(nbytes - curr->length);
			user_incheap(new_space_needed);
			kprintf("We just got %d pages\r\n", new_space_needed/PAGE_SIZE);
			curr->length += new_space_needed;
			head->length += new_space_needed;
			//will return previous
			leftover = (struct memblock*)(curr + nbytes);
			leftover->next = NULL;
			leftover->length = curr->length - nbytes;
			if (prev) prev->next = leftover;
			else head->head = leftover;
			return curr;
		} */
		else if (curr->next == NULL && curr->length < nbytes) {
			//curr is last block, we need to make remainder
			void* new_allocated_block = user_incheap(nbytes);
			if (nbytes % PAGE_SIZE < sizeof(struct memblock)) user_incheap(PAGE_SIZE); // Prevent leftover from not having space for accounting memblock
			leftover = (struct memblock*)(new_allocated_block + nbytes);
			// if (prev) prev->next = leftover;
			// else head->head = leftover;
			curr->next = leftover;
			leftover->next = NULL;
			leftover->length = PAGE_SIZE - (nbytes % PAGE_SIZE);
			return new_allocated_block;
		} else {
			prev = curr;
			curr = curr->next;
		}
	}

    return (void *)SYSERR;
}
