CFLAGS=-m32 -g -mstackrealign
TESTOBJ=test1 test2 test3 test4
THREAD_OBJ=dwthread_queue.o dwthread_local.o jmp.o

all: test1 test2 test3 test4 testjmp

lab1: test1 test4 testjmp

test1: none-dwthread $(THREAD_OBJ)
	gcc $(CFLAGS) -o test1 $(THREAD_OBJ) none_dwthread.o test1.c

test2: dwthread $(THREAD_OBJ)
	gcc $(CFLAGS) -o test2 $(THREAD_OBJ) dwthread.o test2.c

test3: dwthread $(THREAD_OBJ)
	gcc $(CFLAGS) -o test3 $(THREAD_OBJ) dwthread.o test3.c

test4: none-dwthread $(THREAD_OBJ)
	gcc $(CFLAGS) -o test4 $(THREAD_OBJ) none_dwthread.o test4.c

testjmp:
	cd longjmp && make test

dwthread: dwthread.c dwthread.h
	gcc $(CFLAGS) -c -o dwthread.o dwthread.c

none-dwthread: dwthread.c dwthread.h
	gcc $(CFLAGS) -D DWT_NON_PREEMPTIVE -c -o none_dwthread.o dwthread.c
	
dwthread_queue.o: dwthread_queue.c dwthread_queue.h
	gcc $(CFLAGS) -c dwthread_queue.c

dwthread_local.o: dwthread_local.c dwthread_local.h
	gcc $(CFLAGS) -c dwthread_local.c

jmp.o: ./longjmp/jmp.c ./longjmp/jmp.h
	cd longjmp && make

clean:
	-@cd longjmp && make clean
	-@rm -rf *.o  $(TESTOBJ) *.dSYM core *.s
