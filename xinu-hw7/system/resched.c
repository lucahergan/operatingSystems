/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 Assignment 6
 * Implements select_random_process() for the lottery
 * scheduler. resched() uses this instead of a queue.
 *
 * @author David Mathu
 * @author Luca Hergan
 * TA-BOT:MAILTO david.mathu@marquette.edu
 * TA-BOT:MAILTO luca.hergan@marquette.edu
 */
/* Embedded XINU, Copyright (C) 2008,2024.  All rights reserved. */

#include <xinu.h>

/**
 * Selects a random process, according to the number of tickets
 * it has.
 * @return integer pid
 */
int select_random_process() {
    int chosen_pid = 0;
	//walk through process table
	int i;
	uint total_tickets = 0;
	for (i = 0; i < NPROC; i++) {
		if (proctab[i].state != PRFREE)
			total_tickets += proctab[i].tickets;
	}
	if (total_tickets == 0) return 0;
	//pick one
	uint winner_ticket = rand() % total_tickets;
	uint counter = 0;
	for (i = 0; i < NPROC; i++) {
		if (proctab[i].state != PRFREE) {
			counter += proctab[i].tickets;
			if (counter > winner_ticket) {
				chosen_pid = i;
				break;
			}
		}
	}
	return chosen_pid;
}

extern void ctxsw(void *, void *, ulong);
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

    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        //enqueue(currpid, readylist);
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
    //currpid = dequeue(readylist);
	currpid = select_random_process();
    newproc = &proctab[currpid];
    newproc->state = PRCURR;    /* mark it currently running    */

#if PREEMPT
    preempt = QUANTUM;
#endif

    //kprintf("[%d %d]", oldproc-proctab, newproc-proctab); //print former pid and current pid
    ctxsw(&oldproc->ctx, &newproc->ctx, MAKE_SATP(currpid, newproc->pagetable));

    /* The OLD process returns here when resumed. */
    return OK;
}
