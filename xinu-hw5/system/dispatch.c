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
			xtrap((ulong*)frame, cause, 0, (ulong*)program_counter);
		}
		else {
			//frame[CTX_A7] is the system call from register A7
			ulong ret_val = syscall_dispatch(frame[CTX_A7], (ulong*)frame);

			//Change A0 to return value based on what was received from syscall_dispatch
			frame[CTX_A0] = ret_val;

			//Move program counter
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

