#include "dwthread.h"
#include <stdio.h>

#define CNTN 20
static int cntn = 0;

void time_cost()
{
	int i, j;
	volatile int k;

	for (i = 0; i < 3000; i++) {
		for (j = 0; j < 40000; j++) {
			k += i * j;
		}
	}
}

int fun2()
{
	dwt_tid_t tid = dwthread_get_tid();
	printf("\t\t[%d]START\n", tid);


	while (cntn < CNTN) {
		printf("\t\t[%d]do time cost\n", tid);
		time_cost();
		printf("\t\t[%d]after time cost\n", tid);

		cntn += 2;
		printf("\t\t[%d]cnt changed to %d\n", tid, cntn);
	}

	printf("\t\t[%d]START\n", tid);

	return tid;
}

int fun1()
{
	dwt_tid_t tid = dwthread_get_tid();
	printf("\t[%d]START\n", tid);

	// spawn a new thread
	dwt_tid_t tid1 = dwthread_create(fun2, NULL);
	if (tid1 == DWT_TID_NONE) {
		printf("\t[%d]dwthread_create error\n", tid);
	}else {
		printf("\t[%d]create thread %d\n", tid, tid1);
	}

	while (cntn < CNTN) {
		cntn += 1;
		printf("\t[%d]cnt changed to %d\n", tid, cntn);

		printf("\t[%d]do time cost\n", tid);
		time_cost();
		printf("\t[%d]after time cost\n", tid);
	}


	// reap sub thread
	int ret;
	while (dwthread_reap(tid1, &ret) == 0) {
		dwthread_yield();
	}

	printf("\t[%d]%d returned with %d\n", tid, tid1, ret);
	if (tid1 != ret) {
		printf("\t[%d]%d return FAILED\n", tid, tid1);
	}
	printf("\t[%d]RETURN\n", tid);

	return tid;
}



int main()
{
	if (dwthread_init() < 0) {
		printf("[main]dwthread_init error\n");
		return -1;
	}
	dwt_tid_t tid = dwthread_get_tid();
	printf("[main]main tid = %d\n", tid);

	dwt_tid_t tids[3];
	int i;

	for (i = 0; i < 3; i++) {
		tids[i] = dwthread_create(fun1, NULL);

		if (tids[i] == DWT_TID_NONE) {
			printf("[main]dwthread_create error\n");
		}else {
			printf("[main]create thread %d\n", tids[i]);
		}
	}

	int cnt = 0, ret;
	while(cnt < 3) {
		for (i = 0; i < 3; i++) {
			if (dwthread_reap(tids[i], &ret)) {
				printf("[main]%d returned with %d\n", tids[i], ret);
				if (tids[i] != ret) {
					printf("\t[%d]%d return FAILED\n", tid, tids[i]);
				}
				cnt++;
			}
		}

		dwthread_yield();
	}

	return 0;
}

