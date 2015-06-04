#include "dwthread.h"

#define STACK_SIZE	(1<<18)
#define STACK_MASK	0xFFFC0000

#define STACK_NUM	8	

void local_init();
dwt_reg_t get_new_stack(void *info);
void free_stack(dwt_reg_t base);

void store_tid(dwt_reg_t ebp, dwt_tid_t tid);
