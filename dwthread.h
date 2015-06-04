#ifndef DWTHREAD_H
#define DWTHREAD_H

#include <stdint.h>
#include "./longjmp/jmp.h"

typedef unsigned int dwt_reg_t;
typedef void (*fp_t)();
typedef void (*fun)(); 

struct _dwt_task_context_t {
	dwt_reg_t eax;
	dwt_reg_t ebx;
	dwt_reg_t ecx;
	dwt_reg_t edx;
	dwt_reg_t esi;
	dwt_reg_t edi;
	restorer_t jmp_restorer;
};
typedef struct _dwt_task_context_t dwt_task_context_t;

typedef unsigned int dwt_tid_t;
#define DWT_TID_NONE 0

enum _dwt_statue_t {
	DWT_STATUE_RUNNABLE = 0,
	DWT_STATUE_RUNNING,
	DWT_STATUE_RETURNED
};
typedef enum _dwt_statue_t dwt_statue_t;


struct _dwt_task_t {
	dwt_tid_t tid;
	dwt_task_context_t context;
	dwt_reg_t stack_base;
	dwt_statue_t statue;
	int retVal;
};
typedef struct _dwt_task_t dwt_task_t;

typedef void *dwt_tls_t;
typedef int dwt_tls_key_t;

int dwthread_init();
void dwthread_yield();
dwt_tid_t dwthread_create(int (*fun_ptr)(), void *info);
int dwthread_reap(dwt_tid_t tid, int *ptr);

dwt_tid_t dwthread_get_tid();

dwt_tls_key_t dwt_tls_store(dwt_tls_t o);
dwt_tls_t dwt_tls_get(dwt_tls_key_t key);

#endif
