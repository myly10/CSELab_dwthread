CFLAGS=-m32 -g

all: jmp.c jmp.h
	gcc $(CFLAGS) -c jmp.c
	cp jmp.o ../

test: jmp.c jmp.h testjmp.c
	make all
	gcc $(CFLAGS) -o testjmp jmp.o testjmp.c

clean:
	-@rm *.s *.o testjmp
