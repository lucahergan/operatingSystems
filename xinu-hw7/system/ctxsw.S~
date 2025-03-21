/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 * Defines the ctxsw function, which context-switches from
 * one process to another using the two processes' context arrays.
 * @author David Mathu
 * @author Luca Hergan
 * Instructor Dr. Brylow
 * TA-BOT:MAILTO david.mathu@marquette.edu
 * TA-BOT:MAILTO luca.hergan@marquette.edu
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
	mv t0, a0 //old context is in t0
	mv t1, a1 //new context is in t1

	sd s0, 8*CTX_S0(t0)
	sd s1, 8*CTX_S1(t0)
	sd s2, 8*CTX_S2(t0)
	sd s3, 8*CTX_S3(t0)
	sd s4, 8*CTX_S4(t0)
	sd s5, 8*CTX_S5(t0)
	sd s6, 8*CTX_S6(t0)
	sd s7, 8*CTX_S7(t0)
	sd s8, 8*CTX_S8(t0)
	sd s9, 8*CTX_S9(t0)
	sd s10, 8*CTX_S10(t0)
	sd s11, 8*CTX_S11(t0)
	sd ra, 8*CTX_PC(t0)
	sd ra, 8*CTX_RA(t0)
	sd sp, 8*CTX_SP(t0)
// TODO: Switch from old process stack to new process stack.
	// TODO what exactly does this mean? is it fine that we ld sp later from the context array?
// TODO: Restore process state, including at least the
//	callee-save ("non-volatile") registers.
//	These call and restore segments must match the register data
//	layout you choose in create().
	ld s0, 8*CTX_S0(t1)
	ld s1, 8*CTX_S1(t1)
	ld s2, 8*CTX_S2(t1)
	ld s3, 8*CTX_S3(t1)
	ld s4, 8*CTX_S4(t1)
	ld s5, 8*CTX_S5(t1)
	ld s6, 8*CTX_S6(t1)
	ld s7, 8*CTX_S7(t1)
	ld s8, 8*CTX_S8(t1)
	ld s9, 8*CTX_S9(t1)
	ld s10, 8*CTX_S10(t1)
	ld s11, 8*CTX_S11(t1)
	ld a0, 8*CTX_A0(t1)
	ld a1, 8*CTX_A1(t1)
	ld a2, 8*CTX_A2(t1)
	ld a3, 8*CTX_A3(t1)
	ld a4, 8*CTX_A4(t1)
	ld a5, 8*CTX_A5(t1)
	ld a6, 8*CTX_A6(t1)
	ld a7, 8*CTX_A7(t1)
// TODO: Jump to next function.
	ld sp, CTX_SP*8(t1)
	ld t2, CTX_PC*8(t1) //t2 holds new PC
	ld ra, CTX_RA*8(t1)
	//jalr zero, t2, 0
	//mv ra, t2

	mv t0, t2
	beq t0, ra, switch
	li t4, SSTATUS_S_MODE
	csrc sstatus, t4
	csrw sepc, t0

	sret
		switch:
	ret

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
	//ret

// ASSIGNMENT 5 TODO:
// Note: the following code is for the next assignment.  DO NOT uncomment it
//      until you believe your trap handler is working.
// Determine which privilege mode the process should be in

	.end ctxsw
