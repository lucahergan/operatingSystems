/**
 * @file testcases.c
 * @provides testcases
 *
 *
 * Modified by:	David Mathu, Luca Hergan
 *
 * TA-BOT:MAILTO david.mathu@marquette.edu
 * TA-BOT:MAILTO luca.hergan@marquette.edu
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

void powers() {
	print_free_list((memhead*) proctab[currpid].heaptop);
	void* ptr1 = getmem(65536);
	kprintf("getmem(%d) = 0x%08X\r\n", 65536, ptr1);
	void* ptr2 = getmem(1048576);
	kprintf("getmem(%d) = 0x%08X\r\n", 1048576, ptr2);
	void* ptr3 = getmem(1048576);
	kprintf("getmem(%d) = 0x%08X\r\n", 1048576, ptr3);
	print_free_list((memhead*) proctab[currpid].heaptop);
	freemem(ptr1, 65536);
	freemem(ptr2, 1048576);
	freemem(ptr3, 1048576);
	kprintf("freemem() them both\r\n");
	print_free_list((memhead*) proctab[currpid].heaptop);
}
	

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
	uchar c;

	kprintf("===TEST BEGIN===\r\n");
	kprintf("2) Use getmem() to get 0x1100 bytes, and free them\r\n");
	kprintf("3) Several small getmem() and freemem() calls\r\n");
	kprintf("4) Use malloc() to get 0x1100 bytes, and free them\r\n");
	kprintf("5) Mimics powers test case\r\n");
	  
	// TODO: Test your operating system!
	
	int pid;

	c = kgetc();
	switch (c)
	{
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
		case '5':
			pid = create((void *)powers, INITSTK, 1, "myProcess", 0);
			ready(pid, RESCHED_YES);
			break;
		default:
			break;
	}

	kprintf("\r\n===TEST END===\r\n");
	return;
}
