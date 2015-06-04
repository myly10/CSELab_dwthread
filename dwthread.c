#include "dwthread.h"
#include "dwthread_local.h"
#include "dwthread_queue.h"
#include "./longjmp/jmp.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>

#define TID_START 0x1

static dwt_tid_t current_tid;

static restorer_t _dwt_schedule_jmp_storer;
static dwt_tid_t _global_tid = TID_START;

void cleanup();
static void schedule();


int dwthread_init()
{
	local_init(TID_START);
    current_tid = TID_START;

	return 0;
}

dwt_tid_t dwthread_get_tid()
{
    return current_tid;
}

void dwthread_yield()
{
	/*
	 * 1. save the context of caller
	 */
	// we do this before all other works
	// to keep registers untouched
	volatile dwt_reg_t eax, ebx, ecx, edx, esi, edi;
	__asm__("movl %%eax, %0\n\t"
			"movl %%ebx, %1\n\t"
			"movl %%ecx, %2\n\t"
			"movl %%edx, %3\n\t"
			"movl %%esi, %4\n\t"
			"movl %%edi, %5\n\t"
			:"=m"(eax), "=m"(ebx), "=m"(ecx),
			 "=m"(edx), "=m"(esi), "=m"(edi));

	dwt_tid_t cur_tid = dwthread_get_tid();
	dwt_task_t *cur_task = dw_find(cur_tid);

	// TODO: for the first task, init its task_t
	if (cur_task == NULL) {
		cur_task=(dwt_task_t*)malloc(sizeof(dwt_task_t));
		cur_task->tid=cur_tid;
	//	cur_task->context={eax,ebx,ecx,edx,esi,edi};
		cur_task->stack_base=get_new_stack(NULL);
		cur_task->statue=DWT_STATUE_RUNNING;
		dw_inqueue(cur_task);
	}


	/*
	 * 2. set jump and schedule()
	 */
	if (setjump(&cur_task->context.jmp_restorer) == 0) {
		schedule();
	}else { // long jump from schedule(), run the task
		// restore registers
		__asm__("movl %0, %%eax\n\t"
				"movl %1, %%ebx\n\t"
				"movl %2, %%ecx\n\t"
				"movl %3, %%edx\n\t"
				"movl %4, %%esi\n\t"
				"movl %5, %%edi\n\t"
				::"m"(eax), "m"(ebx), "m"(ecx),
				  "m"(edx), "m"(esi), "m"(edi));
	}

	return;
}


/**
  * Generate a new reusable tid
  */
static dwt_tid_t generate_tid()
{
	return ++_global_tid;
}

	
/**
  * Create a new thread, which will start to execute 
  * function fp right after create() returns.
  *
  * para: fp - pointer to the function to be executed.
  *       info - not used currently
  *
  * return: 0 - create new thread successfully
  *         DWT_TID_NONE - failed to create new thread
  */
dwt_tid_t dwthread_create(int (*fp)(), void *info)
{
	/*
	 * TODO: 1. Alloc a new task_t and init its fields.
	 */
	dwt_task_t *task=(dwt_task_t*)malloc(sizeof(dwt_task_t));
	task->tid=generate_tid();
	task->statue=DWT_STATUE_RUNNABLE;

	/*
	 * TODO: 2. Alloc a new stack and fill in the fields
     *          in context of task_t.
     *
     * NOTE: stack_base refers to the HIGHEST address of
     *       stack.
	 */
	task->stack_base=get_new_stack(NULL);
	fp_t *ft=(fp_t*)(task->stack_base);
	*ft=cleanup;
	task->context.jmp_restorer._ebp=task->stack_base;
	task->context.jmp_restorer._esp=task->stack_base;
	task->context.jmp_restorer._eip=fp;

	/*
	 * TODO: 3. Link new task_t into queue
	 */
	if (dw_inqueue(task)==0) return task->tid;
	return DWT_TID_NONE;
}

/**
  * Check whether a specific thread has returned.
  * If it has, save its return value into ptr.
  *
  * return: 0 - if thread has not returned.
  *         1 - if thread has returned.
  */
int dwthread_reap(dwt_tid_t tid, int *ptr)
{
	dwt_task_t *task = dw_find(tid);
	if (task == NULL || (task->statue != DWT_STATUE_RETURNED))
		return 0;

	// store return value
	*ptr = task->retVal;

	// TODO: Remove task_t from queue
    //       and free the task_t
	dw_dequeue(tid);
	free_stack(task->stack_base);
	free(task);

	return 1;
}

/**
 * Hung up the currently running task,
 * set it to 'paused', and find a new
 * runnable task to run.
 */
static void schedule()
{
	/*
	 * TODO: 1. Hung up the current task,
     *          and set its status.
	 */
	dwt_task_t *curTask=dw_find(current_tid);
	if (curTask!=NULL && curTask->statue==DWT_STATUE_RUNNING)
		curTask->statue=DWT_STATUE_RUNNABLE;

	/*
	 * TODO: 2. Find the next to run
	 */
	curTask=dw_next();

	/*
	 * TODO: 3. Get it running
     *          DON'T forget to change current_tid
	 */
	curTask->statue=DWT_STATUE_RUNNING;
	current_tid=curTask->tid;
	longjump(&(curTask->context.jmp_restorer), 1);
}

/**
 * Here is the return point of every thread.
 * It will get the return value and clean up
 * the terminated thread.
 */
void cleanup()
{
	int ret;
	dwt_tid_t tid;
	dwt_task_t *task;

	/*
	 * 1. get return value
	 * NOTE: NO global reference used in this function
	 *       because we must ensure this line is right
	 *       after the return of last thread.
	 */
	__asm__("movl %%eax, %0\n\t"
			:"=g"(ret));

	tid = dwthread_get_tid();
	task = dw_find(tid);
	if (task == NULL) {
		schedule();
	}

	task->statue = DWT_STATUE_RETURNED;
	task->retVal = ret;

	/*
	 * 2. free stack
	 */
	//free_stack(task->stack_base);

	/*
	 * 3. schedule to another thread
	 */
	schedule();
}

















