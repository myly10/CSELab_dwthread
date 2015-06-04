#ifndef DWTHREAD_QUEUE_H
#define DWTHREAD_QUEUE_H

#include "dwthread.h"

int dw_inqueue(dwt_task_t *task);
dwt_task_t *dw_find(dwt_tid_t tid);
dwt_task_t *dw_dequeue(dwt_tid_t tid);
dwt_task_t *dw_next();

#endif
