#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "jmp.h"

/**
 * You should refer to the definitions of 
 * 'setjmp' and 'longjmp' in Linux
 * to implement these two functions.
 */

/**
 * Set a jump point.
 *
 * return:
 *      0 - normally return
 *      else - return from longjump
 */
int setjump(restorer_t *restorer)
{
	uint32_t ebp, esp, eip, caller;//, hint=0;

	// get ebp of setjmp
	__asm__("movl %%ebp, %0\n\t"
			:"=g"(ebp));

	// set restorer with caller's frame,
	// based on the value of %ebp
	restorer->_ebp = *(uint32_t*)ebp;

	// TODO: calculate corresponding %esp and %eip, 
	// and store them in restorer.
	__asm__("movl %%esp, %0\n\t"
			:"=g"(esp));
	restorer->_esp=ebp+8;

	restorer->_eip=*(uint32_t*)(ebp+4);
		
	return 0;
}

/**
 * Jump to the point referred by restorer.
 * This function should NEVER return.
 */
int longjump(restorer_t *restorer, int hint)
{
	uint32_t ebp = restorer->_ebp;
	uint32_t esp = restorer->_esp;
	uint32_t eip = restorer->_eip;

	__asm__("movl %0, %%ebp\n\t"
			"movl %1, %%esp\n\t"
			"pushl %2\n\t"
			"ret\n\t"
			::"r"(ebp), "r"(esp), "r"(eip), "a"(hint));

	return -1;
}

