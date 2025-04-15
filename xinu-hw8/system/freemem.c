/**
 * @file freemem.c
 *
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
	 
	 /*
	 walk from start to back, insert once
	 
	 previous = the previous node, starts as null
	 looking_at = the current node
	 
	 */
	 
	 struct memblock* previous = NULL;
	 struct memblock* looking_at = head->head;
	 
	if ((ulong)looking_at > (ulong)memptr) {
		// first node is already too far, handle manually
		struct memblock* new_first_block = (struct memblock*) memptr;
		//kprintf("//nbytes = 0x%X right here\r\n", nbytes);
		new_first_block->length = nbytes;
		new_first_block->next = looking_at;
		head->head = new_first_block;
		//we want looking_at >= new_first_block
		previous = new_first_block;
	}
	 
	while (looking_at && ((ulong)looking_at < (ulong)memptr)) {
		previous = looking_at;
		looking_at = looking_at->next;
	}
	
	if (previous) if ((ulong)previous + previous->length > (ulong)memptr) {
		//overlap previous block
		return SYSERR;
	}
	if (looking_at) if ((ulong)memptr + nbytes > (ulong)looking_at) {
		//overlap next block
		return SYSERR;
	}
	
	if (previous && ((ulong)previous + previous->length == (ulong)memptr)) {
		block = previous;
		previous->length += nbytes;
	} else {
		((struct memblock*)memptr)->length = nbytes;
		block->next = looking_at->next;
	}
	//kprintf("JUST RAN FREEMEM, prev = 0x%08X, looking_at = 0x%08X\r\n", previous, looking_at);
	//kprintf("Hello\r\n");
	
	head->length += nbytes;

    return OK;
}
