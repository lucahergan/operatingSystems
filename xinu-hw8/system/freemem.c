/**
 * @file freemem.c
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
 * Frees a block of heap-allocated memory.
 *
 * @param memptr
 *      Pointer to memory block allocated with getmem().
 *
 * @param nbytes
 *      Length of memory block, in bytes.  (Same value passed to getmem().)
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail
 *      because of memory corruption or specifying an invalid memory block.
 */
syscall freemem(void *memptr, uint nbytes)
{
    register struct memblock *block, *next, *prev;
    struct memhead *head = NULL;
    ulong top;

    /* make sure block is in heap */
    if ((0 == nbytes)
        || ((ulong)memptr < (ulong)proctab[currpid].heaptop))
    {
        return SYSERR;
    }

    head = (struct memhead *)proctab[currpid].heaptop;
    block = (struct memblock *)memptr;
    nbytes = (uint)roundmb(nbytes);

    /* TODO:
     *      - Find where the memory block should
     *        go back onto the freelist (based on address)
     *      - Find top of previous memblock
     *      - Make sure block is not overlapping on prev or next blocks
     *      - Coalesce with previous block if adjacent
     *      - Coalesce with next block if adjacent
     */
	 
	 struct memblock* previous = (struct memblock*)head;
	 struct memblock* looking_at = head->head;
	 
	while (looking_at && ((ulong)looking_at < (ulong)memptr)) {
		previous = looking_at;
		looking_at = looking_at->next;
	}
	
	if (previous == head) {
		//we are inserting new free block at front
		block->length = nbytes;
		block->next = head->head; //make our block point to what used to be the first node
		head->head = block;
	} else if (previous && (((ulong)previous + previous->length) > (ulong)memptr)) {
		//overlap previous block
		kprintf("Overlap with previous block\r\n");
		return SYSERR;
	} else if (looking_at && (((ulong)memptr + nbytes) > (ulong)looking_at)) {
		//overlap next block
		kprintf("Overlap with next block\r\n");
		return SYSERR;
	} else {
		//previous isn't head, needs to point to this new block
		block->next = previous->next;
		block->length = nbytes;
		previous->next = block;
	}
	
	//New loop through linked list to coalesce
	looking_at = previous;
	while (looking_at) {
		if ((ulong)looking_at + looking_at->length == (ulong)(looking_at->next)) {
			looking_at->length += looking_at->next->length;
			looking_at->next = looking_at->next->next;
		} else looking_at = looking_at->next;
	}
	
	head->length += nbytes;

    return OK;
}
