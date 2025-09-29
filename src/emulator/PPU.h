#ifndef PPU_H
#define PPU_H

#include "types.h"
#include "MemMapping.h"

#include "ROM.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

#define CHRROM_SIZE 0x2000

typedef struct PPU
{
    byte *CHRROM;
    ROM ROM;
    
} PPU;

extern PPU *ppu;

void initPPU(ROM *rom);
void printCHRROM();
#endif