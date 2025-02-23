/**
 * @file dispatch.c
 * @provides dispatch
 *
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

void dispatch(ulong cause, ulong val, ulong *frame, ulong *program_counter) {
    ulong swi_opcode;
    
    if((long)cause > 0) {
        cause = cause << 1;
        cause = cause >> 1;

	int system_call_number = frame[CTX_A7];
	//kprintf("calling dispatch\n");

	//argument 7 from the frame holds what the system call was
	//put return value of system call in frame[a0]
	//you'll need to add exactly 4 to pointer, when u use set_sepc

	if (val != E_ENVCALL_FROM_UMODE) {
		xtrap(frame, cause, (ulong)frame, program_counter); //what is the "address" here the same as frame?
	} else {
		int result = syscall_dispatch(system_call_number, frame);
		frame[0] = result;
		set_sepc((ulong)program_counter + 4);
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
    }
}

