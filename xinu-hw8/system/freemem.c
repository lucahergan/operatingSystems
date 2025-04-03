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
syscall freemem(void *memptr, ulong nbytes)
{
    register struct memblock *block, *next, *prev;
    struct memhead *head = NULL;
    ulong top;

    /* make sure block is in heap */
    if ((0 == nbytes)
        || ((ulong)memptr < (ulong)PROCHEAPADDR))
    {
        return SYSERR;
    }

    head = (struct memhead *)PROCHEAPADDR;
    block = (struct memblock *)memptr;
    nbytes = (ulong)roundmb(nbytes);

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
	 
	 while (true) {
		 if ((ulong)looking_at < (ulong)memptr) {
			 looking_at = looking_at->next;
		 } else if ((ulong)looking_at > (ulong)memptr) { //insert new free block between previous (might be null) and looking_at
			 struct memblock* new_block = (struct memblock*) memptr;
			 new_block->next = looking_at;
			 if (previous != NULL) previous->next = new_block;
			 else head->head = new_block;
			 new_block->length = nbytes;
			 // TODO: check it's not overlapping
			 // TODO: coalesce
		 }
	 }

    return OK;
}
