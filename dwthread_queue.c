#include "dwthread_queue.h"
#include "dwthread.h"
#include <stdio.h>

#define MAX_QUEUE_LEN 1024

static dwt_task_t *task_queue[MAX_QUEUE_LEN] = {NULL};
static int current_slot = 0;
static int next_slot = 0;

/**
 * Add a new thread into the queue.
 */
int dw_inqueue(dwt_task_t *task)
{
	if (current_slot >= MAX_QUEUE_LEN) {
		return -1;
	}else {
		task_queue[current_slot] = task;
		current_slot++;
	}

	return 0;
}

/**
 * Find a specific thread.
 *
 * return:
 *      The task_t structure of thread tid.
 */
dwt_task_t *dw_find(dwt_tid_t tid)
{
	int i;

	for (i = 0; i <= current_slot; i++) {
		if (task_queue[i] && task_queue[i]->tid == tid) {
			return task_queue[i];
		}
	}

	return NULL;
}

/**
 * Get the next runnable thread.
 *
 * This is where the whole schedule policy is
 * implemented.
 */
dwt_task_t *dw_next()
{
    int cnt = 0;

	while(1) {
		next_slot = (next_slot + 1) % MAX_QUEUE_LEN;
		if (task_queue[next_slot] 
				&& task_queue[next_slot]->statue == DWT_STATUE_RUNNABLE) {
			break;
		}

	}

	return task_queue[next_slot];
}


/**
 * Remove a specific thread from queue.
 */
dwt_task_t *dw_dequeue(dwt_tid_t tid)
{
	int i;

	for (i = 0; i <= current_slot; i++) {
		if (task_queue[i] && task_queue[i]->tid == tid) {
			dwt_task_t *tmp = task_queue[i];
			task_queue[i] = NULL;
			return tmp;
		}
	}

	return NULL;
}

