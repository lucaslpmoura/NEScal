#ifndef ROM_H
#define ROM_H
#endif

#include "types.h"

#include "stdio.h"
#include "stdlib.h"

#ifndef ROM_SIZE
#define ROM_SIZE 0x8000
#endif

typedef byte* ROM;

ROM InitROM();
void PrintROM(ROM rom);