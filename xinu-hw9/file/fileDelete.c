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
	printf("file has block # %d\n", file.fn_blocknum);
	
	//Remove from master directory index...?
	struct dirblock* master_directory = supertab->sb_dirlst;
	if (master_directory == NULL) {
		printf("master_directory was null\n");
		signal(supertab->sb_dirlock);
		return SYSERR;
	}
	
	//find which i value has master_directory [i] as this file node
	int i;
	for (i = 0; i < DIRENTRIES; i++) { //find when file #i in master directory is our filenode file
		// if (&(master_directory->db_fnodes[i]) == &(file)) break;
		if (master_directory->db_fnodes[i].fn_blocknum == file.fn_blocknum) break;
	}
	if (i >= DIRENTRIES) {
		printf("Never found file in master directory\n");
		signal(supertab->sb_dirlock);
		return SYSERR;
	}
	printf("File found in master directory at index %d\n", i);
	master_directory->db_fnodes[i].fn_state = FILE_FREE;
	
	// Make sure the disk agrees that this file is gone
	seek(DISK0, supertab->sb_dirlst->db_blocknum);
	if (SYSERR == write(DISK0, supertab->sb_dirlst, sizeof(struct dirblock))) {
		printf("fileDelete write to disk failed\n");
		signal(supertab->sb_dirlock);
		return SYSERR;
	}
	
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
	
    return OK;
}
