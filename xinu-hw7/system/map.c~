/**
 * @file map.c
 * @provides mapPage, mapAddressRange
 *
 * Modified by:
 *
 * TA-BOT:MAILTO
 */
/* Embedded Xinu, Copyright (C) 2013, 2025.  All rights reserved. */

#include <xinu.h>

syscall mapPage(pgtbl pagetable, ulong vaddr, ulong paddr, int attr);

/**
 * Maps a given virtual address range to a corresponding physical address range.
 * @param pagetable    base pagetable
 * @param vaddr        start of the virtual address range.
 *                     This should be truncated to the nearest page boundry.
 * @param paddr        start of the physical address range
 * @param length       length of the range to map
 * @param attr         any attributes to set on the page table entry
 * @return             OK if successful, otherwise a SYSERR
 */
syscall mapAddressRange(pgtbl pagetable, ulong vaddr, ulong paddr,
                        ulong length, int attr)
{
    ulong end;

    // DEBUGGING LINE:
    //    kprintf("mapAddressRange(0x%lX, 0x%lX, 0x%lX, 0x%lX, 0x%lX)\r\n",
    //      pagetable, vaddr, paddr, length,  attr);

    if (length == 0)
    {
        return SYSERR;
    }

    // Round the length, vaddr and paddr to the nearest page size
    length = roundpage(length);
    vaddr = (ulong)truncpage(vaddr);
    paddr = (ulong)truncpage(paddr);
    end = vaddr + length;

    // Loop over the entire range
    for (; vaddr < end; vaddr += PAGE_SIZE, paddr += PAGE_SIZE)
    {
        // Map the individual virtual pages into the table.
        if (mapPage(pagetable, vaddr, paddr, attr) == SYSERR)
        {
            return SYSERR;
        }
    }

    return OK;
}

/**
 * Maps a page to a specific virtual address
 * @param pagetable  base pagetable
 * @param vaddr      virtual address at which the page will reside
 * @param paddr      physical frame that will be mapped at the virtual address
 * @param attr       any attributes to set on the page table entry
 * @return           OK if successful, otherwise a SYSERR
 */
syscall mapPage(pgtbl pagetable, ulong vaddr, ulong paddr, int attr)
{
/**
 * Starting at the base pagetable, traverse the hierarchical page table
 *  structure for the virtual address.  Create pages along the way if they
 *  don't exist.
 */

    /**
    * TODO:
    * For each level in the page table, get the page table entry by masking
    * and shifting the bits in the virtualaddr depending on the level.
    * If the valid bit is set, use that pagetable for the next level
    * Otherwise create the page by calling pgalloc().  Make sure to setup the 
    * page table entry accordingly. Call sfence_vma once finished to flush TLB
    * Once you've traversed all three levels, set the attributes (attr) for
    * the leaf page (don't forget to set the valid bit!)
    */
	
	ulong vpn2 = (vaddr >> 30) & 0x1ff;
	ulong vpn1 = (vaddr >> 21) & 0x1ff;
	ulong vpn0 = (vaddr >> 12) & 0x1ff;
	
	// kprintf("Mapping %d, %d, %d to physical frame %08X\r\n", vpn2, vpn1, vpn0, paddr);
	
	pgtbl level1table;
	if (pagetable[vpn2] & PTE_V) {
		level1table = PTE2PA(pagetable[vpn2]);
	} else {
		level1table = pgalloc();
		pagetable[vpn2] = PA2PTE(level1table) | PTE_V;
	}
	
	pgtbl level0table;
	if (level1table[vpn1] & PTE_V) {
		level0table = PTE2PA(level1table[vpn1]);
	} else {
		level0table = pgalloc();
		level1table[vpn1] = PA2PTE(level0table) | PTE_V;
	}
	
	if (level0table[vpn0] & PTE_V) {
		//ALREADY MAPPED TO SOMETHING - IS THAT OKAY ? Do i need to return SYSERR? Do i need to wipe the old frame or something
		level0table[vpn0] = PA2PTE(paddr) | PTE_V | attr;
	} else {
		level0table[vpn0] = PA2PTE(paddr) | PTE_V | attr;
	}

    //  DEBUGGING LINE:
    //  kprintf("mapPage(pt:0x%X, v:0x%X, p:0x%0X, a:0x%03X)\r\n",
    //          pagetable, vaddr, paddr, attr);

    return OK;
}
