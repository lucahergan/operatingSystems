/**
 * @file dispatch.c
 * @provides dispatch
 * 
 * COSC 3250 - Project 5
 * Performs the trap/execution handler, as well as managing return
 * values and return address. If not an environment call from U-Mode,
 * hand off to xtrap.
 * @author David Mathu
 * @author Luca Hergan
 * TA-BOT:MAILTO david.mathu@marquette.edu
 * TA-BOT:MAILTO luca.hergan@marquette.edu
 */
/* Embedded XINU, Copyright (C) 2008, 2023.  All rights reserved. */


#include <xinu.h>

/**
 * @ingroup process
 * Dispatch the trap or exception handler, called via interrupt.S
 * @param cause  The value of the scause register 
 * @param stval  The value of the stval register  
 * @param frame  The stack pointer of the process that caused the interupt 
 * @param program_counter  The value of the sepc register 
 */
 //Cause is the cause of the exception (register scause)
 //find a7 to find sys call
 //change return value in a0
 //program counter is pointer to code

void dispatch(ulong cause, ulong val, ulong *frame, ulong *program_counter) {
    ulong swi_opcode;
    
    if((long)cause > 0) {
        cause = cause << 1;
        cause = cause >> 1;
	
		if (cause != E_ENVCALL_FROM_UMODE) {
			xtrap((ulong*)frame, cause, val, (ulong*)program_counter);
		}
		else {
			//frame[CTX_A7] is the system call from register A7
			ulong ret_val = syscall_dispatch(frame[CTX_A7], (ulong*)frame);

			//Change A0 to return value based on what was received from syscall_dispatch
			frame[CTX_A0] = ret_val;

			//Move program counter
			set_sepc((ulong)program_counter + 4);
			
			// return MAKE_SATP(currpid, page table for this process)
			//use swap area instead (?)
		}
       /**
	* TODO:
	* Check to ensure the trap is an environment call from U-Mode
	* Find the system call number that's triggered
	* Pass the system call number and any arguments into syscall_dispatch.
	* Make sure to set the return value in the appropriate spot.
	* Update the program counter appropriately with set_sepc
	*
	* If the trap is not an environment call from U-Mode call xtrap
	*/
    } else {
		cause = cause << 1;
		cause = cause >> 1;
		uint irq_num;
		volatile uint *int_sclaim = (volatile uint *)(PLIC_BASE + 0x201004);
		irq_num = *int_sclaim;
		if (cause == I_SUPERVISOR_EXTERNAL) {
			interrupt_handler_t handler = interruptVector[irq_num];
			*int_sclaim = irq_num;
			if (handler) {
				(*handler) ();
			} else {
				kprintf("ERROR: No handler registered for interrupt %u\r\n", irq_num);
				while (1);
			}
		}
	}
	
	//return page
}
