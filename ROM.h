#ifndef ROM_H
#define ROM_H


#include "types.h"

#include "stdio.h"
#include "stdlib.h"

#ifndef ROM_SIZE
#define ROM_SIZE 0x8000
#define HEADERED_ROM_SIZE 0x8010
#endif

typedef byte* ROM;

extern ROM rom;

void initROM();
void printROM(ROM rom);

#endif