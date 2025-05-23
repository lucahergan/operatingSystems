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

/* This fake page table will allow you to test your printPageTable function
 * without having paging completely working.
 */
pgtbl createFakeTable(void){
	pgtbl root = pgalloc();
	pgtbl lvl1 = pgalloc();
	pgtbl lvl0 = pgalloc();

	volatile ulong *pte = &(root[5]);
	*pte = PA2PTE(lvl1) | PTE_V;

	ulong *lvl1pte = &(lvl1[145]);
	*lvl1pte = PA2PTE(lvl0) | PTE_V;

	ulong *lvl0pte = &(lvl0[343]);
	*lvl0pte = PA2PTE(0x1000) | PTE_W | PTE_R | PTE_V;

	ulong *lvl0pte1 = &(lvl0[120]);
	*lvl0pte1 = PA2PTE(0x4000) | PTE_X | PTE_R | PTE_V;

	ulong *lvl0pte2 = &(lvl0[45]);
	*lvl0pte2 = PA2PTE(0x8000) | PTE_X | PTE_R | PTE_V;

	return root;
}

void printPageTable(pgtbl pagetable)
{
	/*
	* TODO: Write a function that prints out the page table.
	* Your function should print out all *valid* page table entries in
	* the page table.  If any of the entries are a link (if the
	* Read/Write/Execute bits aren't set), recursively print that page
	* table.  If it is a leaf, print the page table entry and the
	* physical address is maps to. 
	*/
	kprintf("=========================================\r\n");
	printPageTableRecursive(pagetable, 2, 0);
	kprintf("=========================================\r\n");
}
void printPageTableRecursive(pgtbl pagetable, int depth, ulong vaddr) {
	int i;
	int j;
	for (i = 0; i < PTE_MAX; i++) if (pagetable[i] & PTE_V) {
		if (depth) {
			pgtbl nextTable = PTE2PA(pagetable[i]);
			
			for (j = depth; j < 2; j++) kprintf("     ");
			kprintf("%03d: Next table at %08X\r\n", i, nextTable);
			
			printPageTableRecursive(nextTable, depth-1, (vaddr<<9) + i);
		} else {
			for (j = depth; j < 2; j++) kprintf("     ");
			kprintf("%03d: Physical frame at %08X, permissions ", i, PTE2PA(pagetable[i]));
			if (pagetable[i] & PTE_V) kprintf("v"); else kprintf("-");
			if (pagetable[i] & PTE_R) kprintf("r"); else kprintf("-");
			if (pagetable[i] & PTE_W) kprintf("w"); else kprintf("-");
			if (pagetable[i] & PTE_X) kprintf("x"); else kprintf("-");
			if (pagetable[i] & PTE_U) kprintf("u"); else kprintf("-");
			if (pagetable[i] & PTE_G) kprintf("g"); else kprintf("-");
			if (pagetable[i] & PTE_A) kprintf("a"); else kprintf("-");
			if (pagetable[i] & PTE_D) kprintf("d"); else kprintf("-");
			kprintf("\r\n");
			//kprintf("virtual address %08X maps to physical address %08X\r\n", (vaddr<<9)+i, PTE2PA(pagetable[i]));
		}
	}
}

void myProcess() {
	kprintf("this is my process\r\n");
}

void processThatTriesToAccessBadMemory() {
	kprintf("I am a process who will try to access memory that I shouldn't\r\n");
	int* pointer = 3;
	int value = *(pointer);
	kprintf("I am done, the value at that address is %d\r\n", value);
}

void processThatReadsAndWritesKernelVariables() {
	kprintf("Try to read kernel variable: proctab[0].name = %s\r\n", proctab[0].name);
	kprintf("Going to write to that now\r\n");
	proctab[0].name[0] = 'x';
	proctab[0].name[1] = 'x';
	proctab[0].name[2] = 'x';
	kprintf("Read kernel variable again: proctab[0].name = %s\r\n", proctab[0].name);
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
	uchar c;

	kprintf("===TEST BEGIN===\r\n");
	kprintf("0) Create a user process, print its page table, run process\r\n");
	kprintf("1) Run a process that tries to access a region of memory it shouldn't\r\n");
	kprintf("2) Run a process that reads a kernel variable, then tries to write to a kernel variable\r\n");
	kprintf("3) Try to access content from a null pointer, will cause Null pointer exception\r\n");
	kprintf("A) Create a fake page table, print it, call mapPage several times, then print it again\r\n");
	  
	// TODO: Test your operating system!
	
	int pid;

	c = kgetc();
	switch (c)
	{
		case '0':
			// TODO: Write a testcase that creates a user process
			// and prints out it's page table
			kprintf("Make myProcess\r\n");
			pid = create((void *)myProcess, INITSTK, 1, "myProcess", 0);
			kprintf("Made myProcess\r\n");
			pgtbl myProcessTable = proctab[pid].pagetable;
			printPageTable(myProcessTable);
			kprintf("^^ page table for process %d\r\n", pid);
			ready(pid, RESCHED_YES);
			break;
		case '1':
			// TODO: Write a testcase that demonstrates a user
			// process cannot access certain areas of memory
			pid = create((void *)processThatTriesToAccessBadMemory, INITSTK, 1, "myProcess", 0);
			ready(pid, RESCHED_YES);
			break;
		case '2':
			// TODO: Write a testcase that demonstrates a user
			// process can read kernel variables but cannot write
			// to them
			pid = create((void *)processThatReadsAndWritesKernelVariables, INITSTK, 1, "myProcess", 0);
			ready(pid, RESCHED_YES);
			break;
		case '3':
			// TODO: Extra credit! Add handling in xtrap to detect
			// and print out a Null Pointer Exception.  Write a
			// testcase that demonstrates your OS can detect a
			// Null Pointer Exception.
		  int* pointer = 0;
		  kprintf("%d\r\n", *pointer);
			break;
		case 'A':
			// Create a fake page table, print its content, use magePage to map
			// certain virtual addresses to physical memory, then print its content
			// again.
			pgtbl fakeTable = createFakeTable();
			printPageTable(fakeTable);
			kprintf("\n\n");
			mapPage(fakeTable, (3<<12), 0xA000, PTE_R | PTE_W); //map (0, 0, 3) -> 0xA000
			mapPage(fakeTable, (10<<12), 0xB000, PTE_R | PTE_W); //map (0, 0, 10) -> 0xB000
			//mapPage(fakeTable, (((((5<<9)+20)<<9)+30)<<12), 0xC000, PTE_R | PTE_W); //map (5, 20, 30) -> 0xC000
			kprintf("\n\n");
			printPageTable(fakeTable);
		default:
			break;
	}

	kprintf("\r\n===TEST END===\r\n");
	return;
}
