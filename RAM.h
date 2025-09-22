#ifndef RAM_H
#define RAM_H
#endif

#include "types.h"

#include "stdio.h"

#ifndef RAM_SIZE
#define RAM_SIZE 0x800
#endif

typedef byte* RAM;

RAM InitRAM();
void PrintRAM(RAM ram);