#ifndef RAM_H
#define RAM_H


#include "types.h"
#include "stdio.h"
#include <stdlib.h>

#define RAM_SIZE 0x800
#define STACK_START_ADDR 0x100

typedef byte* RAM;

RAM InitRAM();
void PrintRAM(RAM ram);

#endif