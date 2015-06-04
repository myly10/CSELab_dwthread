#include "dwthread.h"
#include <stdio.h>
#include <unistd.h>

#define CNTN 20
static int cntn = 0;

void time_cost()
{
	int i, j;
	volatile int k;

	for (i = 0; i < 30000; i++) {
		for (j = 0; j < 40000; j++) {
			k += i * j;
		}
	}
}

int fun_read()
{
	dwt_tid_t tid = dwthread_get_tid();
	printf("\t\t[%d]read START\n", tid);

	char buf[1024];
	int i, n;

	for (i = 0; i < 3; i++) {
		n = read(STDIN_FILENO, buf, 1023);
		buf[n - 1] = '\0';
		printf("(%s)\n", buf);
	}

	printf("\t\t[%d]read RETURN\n", tid);

	return tid;
}

int fun_run()
{
	dwt_tid_t tid = dwthread_get_tid();
	printf("\t[%d]START\n", tid);

	time_cost();

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

	// create both read and run threads
	dwt_tid_t readtid, runtid;

	readtid = dwthread_create(fun_read, NULL);
	if (readtid == DWT_TID_NONE) {
		printf("[main]create read thread failed\n");
	}
	runtid = dwthread_create(fun_run, NULL);
	if (runtid == DWT_TID_NONE) {
		printf("[main]create run thread failed\n");
	}

	// wait for both to terminate
	int ret, cnt = 0;

	while(1) {
		if (dwthread_reap(readtid, &ret)) {
			printf("[main]read returned with %d\n", ret);
			cnt++;
		}
		if (dwthread_reap(runtid, &ret)) {
			printf("[main]run returned with %d\n", ret);
			cnt++;
		}
		if (cnt == 2)
			break;
	}

	return 0;
}

