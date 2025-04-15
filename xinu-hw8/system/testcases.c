/**
 * @file testcases.c
 * @provides testcases
 *
 *
 * Modified by:	
 *
 * TA-BOT:MAILTO 
 *
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <xinu.h>

void print_free_list(struct memhead* mh) {
	struct memblock* block = mh->head;
	kprintf("   FREE LIST (length: 0x%08X)\r\n", mh->length);
	while (block) {
		kprintf("   At 0x%08X (length: 0x%08X), goes to 0x%08X\r\n", block, block->length, (ulong)block + (ulong)(block->length));
		block = block->next;
	}
}

void processWithMallocAndFree() {
	print_free_list((memhead*) proctab[currpid].heaptop);
	void* pointer = malloc(0x100);
	kprintf("I just malloced 0x100 bytes, they're at %08X\r\n", pointer);
	print_free_list((memhead*) proctab[currpid].heaptop);
	free(pointer);
	kprintf("I just freed them\r\n");
	print_free_list((memhead*) proctab[currpid].heaptop);
}

void processThatRequestsLotsOfMemory() {
	print_free_list((memhead*) proctab[currpid].heaptop);
	void* pointer = malloc(0x1100);
	kprintf("I just malloced 0x100 bytes, they're at %08X\r\n", pointer);
	print_free_list((memhead*) proctab[currpid].heaptop);
	free(pointer);
	kprintf("I just freed them\r\n");
	print_free_list((memhead*) proctab[currpid].heaptop);
}
	

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
	uchar c;

	kprintf("===TEST BEGIN===\r\n");
	kprintf("Test cases from project 8\r\n");
	// kprintf("0) Manually set up a memory heap, request 100 bytes, then free 100 bytes\r\n");
	kprintf("1) Create process that mallocs and frees. Prints free list before/after every operation\r\n");
	kprintf("2) Same as above, but takes more than a page\r\n");
	  
	// TODO: Test your operating system!
	
	int pid;

	c = kgetc();
	switch (c)
	{
		/* case '0':
			proctab[0].heaptop = pgalloc();
			memhead* mh = (memhead*) proctab[0].heaptop;
			mh->head = mh + 1;
			mh->length = PAGE_SIZE - sizeof(struct memhead);
			mh->base = mh->head;
			mh->bound = ((ulong)mh) + PAGE_SIZE;
			mh->head->next = NULL;
			mh->head->length = mh->length;
			kprintf("Free list right now:\r\n");
			print_free_list(mh);
			void* my_memory = malloc(0x100); //request 0x100 bytes
			kprintf("Free list after I requested 0x100 bytes:\r\n");
			print_free_list(mh);
			kprintf("My request 0x100 bytes are at %08X\r\n", my_memory);
			free(my_memory);
			kprintf("Free list after I freed my 0x100 bytes:\r\n");
			print_free_list(mh);
			break; */
		case '1':
			pid = create((void *)processWithMallocAndFree, INITSTK, 1, "myProcess", 0);
			ready(pid, RESCHED_YES);
			break;
		case '2':
			pid = create((void *)processThatRequestsLotsOfMemory, INITSTK, 1, "myProcess", 0);
			ready(pid, RESCHED_YES);
			break;
		default:
			break;
	}

	kprintf("\r\n===TEST END===\r\n");
	return;
}
