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
	if (fbc == NULL) return SYSERR;
	struct dentry* phw = psuper->sb_disk;
	if (phw == NULL) return SYSERR;
	int diskfd = phw - devtab;
	
	//Traverse to last fbc
	while (fbc->fbc_next) {
		fbc = fbc->fbc_next;
	}
	
	if (fbc->fbc_count < FREEBLOCKMAX) {
		//Add this block number to the list
		fbc->fbc_free[fbc->fbc_count++] = block;
		//Edit fbc's next pointer (swizzle!), copy to disk, then fix the version in RAM again
		//Because we're at end of linked list, fbc->fbc_next is necesarilly NULL...So do we really have to swizzle...
		seek(diskfd, fbc->fbc_blocknum);
		if (SYSERR == write(diskfd, fbc, sizeof(struct fbcnode))) {
			signal(psuper->sb_freelock);
			return SYSERR;
		}
		
	} else {
		//Need new fbc block... let's use the block itself!
		//Set fbc to point to the block in disk. Swizzle
		fbc->fbc_next = block;
		seek(diskfd, fbc->fbc_blocknum);
		if (SYSERR == write(diskfd, fbc, sizeof(struct fbcnode))) {
			signal(psuper->sb_freelock);
			return SYSERR;
		}
		
		//Load block fbc_node from block number
		struct fbcnode* new_fbc = NULL; //TODO
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
		
		//Now do the same operation as above
		fbc = new_fbc;
		fbc->fbc_free[fbc->fbc_count++] = block;
		seek(diskfd, fbc->fbc_blocknum);
		if (SYSERR == write(diskfd, fbc, sizeof(struct fbcnode))) {
			signal(psuper->sb_freelock);
			return SYSERR;
		}
	}
	
	signal(psuper->sb_freelock);
	
	return OK;
	
    return SYSERR;
}
