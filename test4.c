#include <stdio.h>
#include "dwthread.h"

int fun1()
{
    int i;
    dwt_tid_t tid = dwthread_get_tid();

    for (i = 0; i < 3; i++) {
        printf("[%d] is running\n", tid);

        dwthread_yield();
    }

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
				cnt++;
			}
		}

		dwthread_yield();
	}

	return 0;
}



