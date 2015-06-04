#include <stdio.h>
#include <stdlib.h>
#include "jmp.h"

static restorer_t rest;

void foo()
{
    printf("Before longjump\n");

    longjump(&rest, 1);

    printf("ERROR: longjump returned\n");
}

int main()
{
    printf("Before jmp\n");

    if (setjump(&rest) == 0) {
        printf("Setjump OK\n");

        foo();

        printf("ERROR: foo returned\n");
        exit(1);
    }else {
        printf("After longjump\n");
    }

    return 0;
}

