#ifndef PPU_H
#define PPU_H

#include "PPU_Registers.h"

#include "types.h"
#include "MemMapping.h"

#include "ROM.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

#define CHRDATA_SIZE 0x2000
#define CHRDATA_START_ADDR 0x8010

#define VRAM_SIZE  0x800
#define PATTERNTABLE_FINAL_ADDR 0x2000
#define NAMETABLE_FINAL_ADDR 0x3F00

#define PALLETE_RAM_SIZE 0x20


#define CHR_DATA_SIZE_BIT 5
#define HORIZONTAL_MIRRORING_BIT 6

typedef struct PPU
{
    bool WL; // Write Latch
    

    addr transferAddr; // Transfer Address
    addr tempAddr; //  Temporary Addres (used to read the VRAM Address)
    

    byte *header;
    ROM rom;

    byte *vRam;
    addr vRamAddr; // VRAM Address
    bool vRamInc32Mode; // Increase by 32 flag

    byte *chrData;
    byte *palleteRam;

    byte readBuffer;
} PPU;

extern PPU *ppu;

void initPPU(ROM *rom);

byte readPPU(addr address);
void writePPU(addr address, byte value);

void printCHRROM();
#endif