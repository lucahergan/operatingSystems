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
			kprintf("%03d: Physical frame at %08X\r\n", i, PTE2PA(pagetable[i]));
			//kprintf("virtual address %08X maps to physical address %08X\r\n", (vaddr<<9)+i, PTE2PA(pagetable[i]));
		}
	}
}

void myProcess() {
	kprintf("this is my process\r\n");
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
	uchar c;

	kprintf("===TEST BEGIN===\r\n");

	// TODO: Test your operating system!

	//c = kgetc();
	c = 'A';
	switch (c)
	{
		case '0':
			// TODO: Write a testcase that creates a user process
			// and prints out it's page table
			kprintf("Make myProcess\r\n");
			int pid = ready(create((void *)myProcess, INITSTK, 1, "myProcess", 0), RESCHED_NO);
			kprintf("Made myProcess\r\n");
			pgtbl myProcessTable = proctab[pid].pagetable;
			printPageTable(myProcessTable);
			break;
		case '1':
			// TODO: Write a testcase that demonstrates a user
			// process cannot access certain areas of memory
			break;
		case '2':
			// TODO: Write a testcase that demonstrates a user
			// process can read kernel variables but cannot write
			// to them
			break;
		case '3':
			// TODO: Extra credit! Add handling in xtrap to detect
			// and print out a Null Pointer Exception.  Write a
			// testcase that demonstrates your OS can detect a
			// Null Pointer Exception.
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
