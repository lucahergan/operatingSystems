/* fileDelete.c - fileDelete */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/* David Mathu                                                     */
/* and                                                             */
/* Luca Hergan                                                     */
/*                                                                 */
/* TA-BOT:MAILTO david.mathu@marquette.edu                         */
/* TA-BOT:MAILTO luca.hergan@marquette.edu                         */

#include <xinu.h>

/*------------------------------------------------------------------------
 * fileDelete - Delete a file.
 *------------------------------------------------------------------------
 */
devcall fileDelete(int fd) {
	
	if (isbadfd(fd)) return SYSERR;
    if ((NULL == supertab) || (NULL == filetab)) return SYSERR;
	
    // TODO: Unlink this file from the master directory index,
    //  and return its space to the free disk block list.
    //  Use the superblock's locks to guarantee mutually exclusive
    //  access to the directory index.
	printf("fileDelete run\n");
	wait(supertab->sb_dirlock);
	
	struct filenode file = filetab[fd]; //Get filenode
	//Remove from master directory index...?
	
	//Return block to free list, and error check
	if (SYSERR == sbFreeBlock(&(supertab[SUPERBLOCKNUM]), file.fn_blocknum)) {
		printf("sbFreeBlock failed\n");
		signal(supertab->sb_dirlock);
		return SYSERR;
	}
	file.fn_length = 0;
	file.fn_cursor = 0;
	file.fn_blocknum = 0;
	file.fn_name[0] = '\0';
	free(file.fn_data); //it was malloced
	file.fn_state = FILE_FREE;
	
	signal(supertab->sb_dirlock);
	
	//wait(sb_dirlock);
	
    return OK;
}
