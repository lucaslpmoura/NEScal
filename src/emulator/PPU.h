#ifndef PPU_H
#define PPU_H

#include "PPU_Registers.h"

#include "types.h"
#include "MemMapping.h"

#include "ROM.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

#define CHRROM_SIZE 0x2000

#define VRAM_SIZE  0x800
#define PATTERNTABLE_FINAL_ADDR 0x2000
#define NAMETABLE_FINAL_ADDR 0x3F00

#define PALLETE_RAM_SIZE 0x20

typedef struct PPU
{
    bool WL; // Write Latch
    

    addr transferAddr; // Transfer Address
    addr tempAddr; //  Temporary Addres (used to read the VRAM Address)
    addr vRamAddr; // VRAM Address

    ROM rom;

    byte *vRam;
    byte *chrRom;
    byte *palleteRam;
    
} PPU;

extern PPU *ppu;

void initPPU(ROM *rom);

void writePPU(addr address, byte value);

void printCHRROM();
#endif