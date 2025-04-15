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
	kprintf("\tFREE LIST (length: 0x%08X)\r\n", mh->length);
	while (block) {
		kprintf("\tAt 0x%08X (length: 0x%X), goes until 0x%08X, points to 0x%08X\r\n", block, block->length, (ulong)block + (ulong)(block->length), block->next);
		block = block->next;
	}
}

void processThatRequestsLotsOfMemory(ulong nbytes) {
	print_free_list((memhead*) proctab[currpid].heaptop);
	void* pointer = getmem(nbytes);
	kprintf("I just got 0x%08X bytes, they're at %08X\r\n", nbytes, pointer);
	print_free_list((memhead*) proctab[currpid].heaptop);
	freemem(pointer, nbytes);
	kprintf("I just freed them\r\n");
	print_free_list((memhead*) proctab[currpid].heaptop);
}

void processThatRequestsLotsOfMemoryUsingMalloc(ulong nbytes) {
	print_free_list((memhead*) proctab[currpid].heaptop);
	void* pointer = malloc(nbytes);
	kprintf("I just MALLOCED 0x%08X bytes, they're at %08X\r\n", nbytes, pointer);
	print_free_list((memhead*) proctab[currpid].heaptop);
	free(pointer);
	kprintf("I just freed them\r\n");
	print_free_list((memhead*) proctab[currpid].heaptop);
}

void processGetmemFreemem() {
	print_free_list((memhead*) proctab[currpid].heaptop);
	void* ptr1 = getmem(256);
	kprintf("getmem(256) = 0x%08X\r\n", ptr1);
	void* ptr2 = getmem(256);
	kprintf("getmem(256) = 0x%08X\r\n", ptr2);
	print_free_list((memhead*) proctab[currpid].heaptop);
	kprintf("Free the first one\r\n");
	freemem(ptr1, 256);
	print_free_list((memhead*) proctab[currpid].heaptop);
	kprintf("Free the second one\r\n");
	freemem(ptr2, 256);
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
	kprintf("2) getmem() calls that will take more than a page\r\n");
	kprintf("3) getmem() calls\r\n");
	kprintf("4) getmem() calls\r\n");
	  
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
		case '2':
			pid = create((void *)processThatRequestsLotsOfMemory, INITSTK, 1, "myProcess", 1, 0x1100);
			ready(pid, RESCHED_YES);
			break;
		case '3':
			pid = create((void *)processGetmemFreemem, INITSTK, 1, "myProcess", 0);
			ready(pid, RESCHED_YES);
			break;
		case '4':
			pid = create((void *)processThatRequestsLotsOfMemoryUsingMalloc, INITSTK, 1, "myProcess", 1, 0x1100);
			ready(pid, RESCHED_YES);
			break;
		default:
			break;
	}

	kprintf("\r\n===TEST END===\r\n");
	return;
}
