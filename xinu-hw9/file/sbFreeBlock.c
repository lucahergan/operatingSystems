/* sbFreeBlock.c - sbFreeBlock */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/*                                                                 */
/* and                                                             */
/*                                                                 */
/*                                                                 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * sbFreeBlock - Add a block back into the free list of disk blocks.
 *------------------------------------------------------------------------
 */
devcall sbFreeBlock(struct superblock *psuper, int block)
{
    // TODO: Add the block back into the filesystem's list of
    //  free blocks.  Use the superblock's locks to guarantee
    //  mutually exclusive access to the free list, and write
    //  the changed free list segment(s) back to disk.
	
	wait(psuper->sb_freelock);
	
	//Get fbc
	if (psuper == NULL) return SYSERR;
	if (psuper->sb_disk == NULL) return SYSERR;
	struct fbcnode* fbc = psuper->sb_freelst;
	struct dentry* phw = psuper->sb_disk;
	if (phw == NULL) return SYSERR;
	int diskfd = phw - devtab;
	if (fbc == NULL) {
		//No free list at all - turn block into the head
		fbc = (struct fbcnode*) malloc(sizeof(struct fbcnode));
		if (fbc == NULL)  {
			signal(psuper->sb_freelock);
			return SYSERR;
		}
		fbc->fbc_blocknum = block;
		fbc->fbc_count = 0;
		fbc->fbc_next = NULL;
		int i;
		for (i = 0; i < FREEBLOCKMAX; i++) fbc->fbc_free[i] = 0;
		seek(diskfd, fbc->fbc_blocknum);
		if (SYSERR == write(diskfd, fbc, sizeof(struct fbcnode))) {
			signal(psuper->sb_freelock);
			return SYSERR;
		}
		psuper->sb_freelst = (struct fbcnode*)block;
		seek(diskfd, psuper->sb_blocknum);
		if (SYSERR == write(diskfd, psuper, sizeof(struct superblock))) {
			signal(psuper->sb_freelock);
			return SYSERR;
		}
		psuper->sb_freelst = fbc;
		signal(psuper->sb_freelock);
		return OK;
	}
	
	//Traverse to last fbc
	while (fbc->fbc_next) {
		fbc = fbc->fbc_next;
	}
	
	if (fbc->fbc_count < FREEBLOCKMAX) {
		//Tail has room - just add it
		fbc->fbc_free[fbc->fbc_count++] = block;
		//Commit changes to tail (no swizzle, next is NULL == 0)
		seek(diskfd, fbc->fbc_blocknum);
		if (SYSERR == write(diskfd, fbc, sizeof(struct fbcnode))) {
			signal(psuper->sb_freelock);
			return SYSERR;
		}
		
	} else {
		//Tail is full, make new_fbc out of the given block, and point fbc to it
		fbc->fbc_next = (struct fbcnode*)block;
		seek(diskfd, fbc->fbc_blocknum);
		if (SYSERR == write(diskfd, fbc, sizeof(struct fbcnode))) {
			signal(psuper->sb_freelock);
			return SYSERR;
		}
		struct fbcnode* new_fbc = (struct fbcnode*) malloc(sizeof(struct fbcnode));
		if (new_fbc == NULL)  {
			signal(psuper->sb_freelock);
			return SYSERR;
		}
		fbc->fbc_next = new_fbc;
		new_fbc->fbc_blocknum = block;
		new_fbc->fbc_count = 0;
		new_fbc->fbc_next = NULL;
		int i;
		for (i = 0; i < FREEBLOCKMAX; i++) new_fbc->fbc_free[i] = 0;
		
		//Now write new_fbc to disk. fbc_next does not need swizzling
		seek(diskfd, new_fbc->fbc_blocknum);
		if (SYSERR == write(diskfd, new_fbc, sizeof(struct fbcnode))) {
			signal(psuper->sb_freelock);
			return SYSERR;
		}
	}
	
	signal(psuper->sb_freelock);
	
	return OK;
	
    return SYSERR;
}
