/**
 * @file     xsh_test.c
 * @provides xsh_test
 *
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * Shell command (test) is testing hook.
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
command xsh_test(int nargs, char *args[])
{
	/* //TODO: Test your O/S.
	printf("This is where you should put some testing code.\n");
	int fd;
	char c = 1;
	
	fileCreate("myfile");
	
	//write "test" into myfile
	fd = fileOpen("myfile");
	filePutChar(fd, 't');
	filePutChar(fd, 'e');
	filePutChar(fd, 's');
	filePutChar(fd, 't');
	filePutChar(fd, EOF);
	fileClose(fd);
	
	//print myfile
	printf("myfile contains \"");
	fd = fileOpen("myfile");
	c = fileGetChar(fd);
	while (c != EOF && c != '\0') {
		printf("%c", c);
		c = fileGetChar(fd);
	}
	printf("\"\n");
	fileClose(fd);
	
	//delete it
	fd = fileOpen("myfile");
	if (SYSERR == fileDelete(fd)) {
		printf("fileDelete returned SYSERR :(\n");
	} else printf("Deleted myfile\n");
	
	//print myfile - hopefully doesn't print "test" still
	fd = fileOpen("myfile");
	if (fd == SYSERR) printf("myfile no longer exists\n");
	else printf("fileOpen() didn't return syserr... myfile still exists ?\n");
	fileClose(fd); */
	
	/* printFreeList();
	printf("FIVE GETS AND FREES (getc between iterations)\n");
	int i;
	char c;
	for (i = 0; i < 5; i++) {
		c = getc(CONSOLE);
		int block = sbGetBlock(supertab);
		sbFreeBlock(supertab, block);
		printFreeList();
	}
	printf("get ALL the blocks\n");
	for (i = 0; i < DISKBLOCKTOTAL - 2; i++) {
		printf("iteration %d\n", i);
		int block = sbGetBlock(supertab);
	}
	printFreeList();
	c = getc(CONSOLE);
	printf("return ALL the blocks\n");
	for (i = 2; i < DISKBLOCKTOTAL; i++) {
		printf("iteration %d\n", i);
		sbFreeBlock(supertab, i);
	}
	printFreeList(); */
	
	for (int i = 0; i < 30; i++) {
		//create file #i
		char* filename = "XXfile";
		filename[0] = '0' + i/10;
		filename[1] = '0' + i%10;
		printf("Going to fileCreate(%s)\n", filename);
		int fd = fileCreate(filename);
		fileClose(fd);
	}
	
    return OK;
}
