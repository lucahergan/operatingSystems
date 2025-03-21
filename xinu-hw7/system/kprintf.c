/*
 * COSC 3250 - Project 3
 * Implements kgetc, kcheckc, kungetc, and kputc, using the
 * given NS16550 UART struct and the ungetArray.
 * @author David Mathu
 * @author Luca Hergan
 * TA-BOT:MAILTO david.mathu@marquette.edu
 * TA-BOT:MAILTO luca.hergan@marquette.edu
 */

#include <xinu.h>

#define UNGETMAX 10             /* Can un-get at most 10 characters. */
static unsigned char ungetArray[UNGETMAX];
unsigned int  bufp;

syscall kgetc()
{
	volatile struct ns16550_uart_csreg *regptr;
	regptr = (struct ns16550_uart_csreg *)UART_BASE;

	// First, check the unget buffer for a character.
	// Otherwise, check UART line status register, and
	// once there is data ready, get character c.
	if (bufp) {
		bufp--;
		return ungetArray[bufp];
	}
	while (!(regptr->lsr & UART_LSR_DR));
	return regptr->buffer;
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
	volatile struct ns16550_uart_csreg *regptr;
	regptr = (struct ns16550_uart_csreg *)UART_BASE;
	// Check the unget buffer and the UART for characters.
	if (bufp) return 1;
	if (regptr->lsr & UART_LSR_DR) return 1;
	return 0;
}

/**
 * kungetc - put a serial character "back" into a local buffer.
 * @param c character to unget.
 * @return c on success, SYSERR on failure.
 */
syscall kungetc(unsigned char c)
{
	// Check for room in unget buffer, put the character in or discard.
	if (bufp < UNGETMAX) {
		ungetArray[bufp] = c;
		bufp++;
		return c;
	}
	return SYSERR;
}

syscall kputc(uchar c)
{
	volatile struct ns16550_uart_csreg *regptr;
	regptr = (struct ns16550_uart_csreg *)UART_BASE;
	// Check UART line status register.
	// Once the Transmitter FIFO is empty, send character c.
	while (!(regptr->lsr & UART_LSR_TEMT));
	regptr->buffer = c;
	return c;
}

syscall kprintf(const char *format, ...)
{
	int retval;
	va_list ap;
	va_start(ap, format);
	retval = _doprnt(format, ap, (int (*)(long, long))kputc, 0);
	va_end(ap);
	return retval;
}
