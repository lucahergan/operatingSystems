/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 Assignment 6
 */
/* Embedded XINU, Copyright (C) 2008,2024.  All rights reserved. */

#include <xinu.h>

/**
 * Selects a random process, according to the number of tickets
 * it has.
 * @return pointer to selected PCB
 */
pcb* select_random_process() {
    pcb *newproc;
	//walk through process table
	int i;
	uint total_tickets = 0;
	for (i = 0; i < NPROC; i++) {
		if (proctab[i].state != PRFREE)
			total_tickets += proctab[i].tickets;
	}
	//pick one
	uint winner_ticket = rand() % total_tickets;
	uint counter = 0;
	for (i = 0; i < NPROC; i++) {
		if (proctab[i].state != PRFREE) {
			counter += proctab[i].tickets;
			if (counter > winner_ticket) newproc = &proctab[i];
		}
	}
	return newproc;
}

extern void ctxsw(void *, void *);
/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate 
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */
syscall resched(void)
{
    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */

    oldproc = &proctab[currpid];
	newproc = select_random_process();

    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        enqueue(currpid, readylist);
    }

    /**
     * We recommend you use a helper function for the following:
     * TODO: Get the total number of tickets from all processes that are in
     * current and ready states.
     * Use random() function to pick a random ticket. 
     * Traverse through the process table to identify which proccess has the
     * random ticket value.  Remove process from queue.
     * Set currpid to the new process.
     */
    currpid = dequeue(readylist);
    newproc = &proctab[currpid];
    newproc->state = PRCURR;    /* mark it currently running    */

#if PREEMPT
    preempt = QUANTUM;
#endif

    ctxsw(&oldproc->ctx, &newproc->ctx);

    /* The OLD process returns here when resumed. */
    return OK;
}
