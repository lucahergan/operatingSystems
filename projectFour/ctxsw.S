/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2025.  All rights reserved. */

#include <riscv.h>

.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldcontext, &newcontext)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the program counter for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldcontext address of outgoing process context save area
 * @param  &newcontext address of incoming process context save area
 * @return special case -- see above
 */
	.func ctxsw
ctxsw:

// TODO: Save process state, including at least the 
//	callee-save ("non-volatile") registers.
	sd s0, 4*CTX_S0(a0)
	sd s1, 4*CTX_S1(a0)
	sd s2, 4*CTX_S2(a0)
	sd s3, 4*CTX_S3(a0)
	sd s4, 4*CTX_S4(a0)
	sd s5, 4*CTX_S5(a0)
	sd s6, 4*CTX_S6(a0)
	sd s7, 4*CTX_S7(a0)
	sd s8, 4*CTX_S8(a0)
	sd s9, 4*CTX_S9(a0)
	sd s10, 4*CTX_S10(a0)
	sd s11, 4*CTX_S11(a0)
	sd ra, 4*CTX_RA(a0)
	sd sp, 4*CTX_SP(a0)
// TODO: Switch from old process stack to new process stack.
	ld sp, 4*CTX_SP(a1)
// TODO: Restore process state, including at least the
//	callee-save ("non-volatile") registers.
//	These call and restore segments must match the register data
//	layout you choose in create().
	ld s0, 4*CTX_S0(a1)
	ld s1, 4*CTX_S1(a1)
	ld s2, 4*CTX_S2(a1)
	ld s3, 4*CTX_S3(a1)
	ld s4, 4*CTX_S4(a1)
	ld s5, 4*CTX_S5(a1)
	ld s6, 4*CTX_S6(a1)
	ld s7, 4*CTX_S7(a1)
	ld s8, 4*CTX_S8(a1)
	ld s9, 4*CTX_S9(a1)
	ld s10, 4*CTX_S10(a1)
	ld s11, 4*CTX_S11(a1)
	ld ra, 4*CTX_RA(a1)
// TODO: Jump to next function.
	//jalr ra, 4*CTX_PC(a1)
	//jalr 4*CTX_RA(a0), 
//	Note: there are two important values at play here --
//	the address of the process's function that we want to end up in
//	once we leave this ctxsw(), and the value we want to have in the
//	return address register.  Those are almost always the same value, except
//	for the very first time we ctxsw() to a new process, when they
//	are different.  See create() for more details.

// Note: This ret line immediately returns from whence we came.  This
//	is here as a placeholder so that you don't crash your O/S when
//	you call ctxsw() before it is complete.  But, this line of code
//	is not actually a part of a completed solution.
	ret

// ASSIGNMENT 5 TODO:
// Note: the following code is for the next assignment.  DO NOT uncomment it
//      until you believe your trap handler is working.
// Determine which privilege mode the process should be in
//	ld t0, (**REGISTER THAT HOLDS NEW CONTEXT PROGRAM COUNTER**)
//	beq t0, ra, switch
//	li t4, SSTATUS_S_MODE
//	csrc sstatus, t4
//	csrw sepc, t0
//
//	sret
//switch:
//	ret

	.end ctxsw
