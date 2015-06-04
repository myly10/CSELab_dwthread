#include "dwthread_local.h"

void local_init()
{
    // NOTE: Will be implemented in lab2...
}

/**
 * Spare a stack of STACK_SIZE for newly-created thread.
 *
 * return:
 *      The HIGHEST address of new stack.
 *
 * NOTE: info is currently not used.
 */
dwt_reg_t get_new_stack(void *info)
{
    // TODO: alloc a new stack
    return (dwt_reg_t)malloc(STACK_SIZE)+STACK_SIZE-sizeof(void(*)());
}

/**
 * Free the stack allocated by get_new_stack.
 */
void free_stack(dwt_reg_t base)
{
    // TODO: note that base is the HIGHEST address of stack
	if (base==0) return;
    free((void*)(base-STACK_SIZE+sizeof(void(*)())));
}

dwt_tls_key_t dwt_tls_store(dwt_tls_t o)
{
    // NOTE: Will be implemented in lab2...
    return 0;
}

dwt_tls_t dwt_tls_get(dwt_tls_key_t key)
{
    // NOTE: Will be implemented in lab2...
    return 0;
}

