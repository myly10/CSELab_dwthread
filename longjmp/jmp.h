#ifndef JMP_H
#define JMP_H

#include <stdint.h>

struct RESTORER {
	uint32_t _ebp;
	uint32_t _esp;
	uint32_t _eip;
};

typedef struct RESTORER restorer_t;

int setjump(restorer_t *restorer);
int longjump(restorer_t *restorer, int hint);

#endif
