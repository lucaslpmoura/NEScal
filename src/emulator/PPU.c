#include "PPU.h"

PPU *ppu = NULL;

void initPPU(ROM *rom){
    ppu = (PPU*) malloc(sizeof(PPU));

    ppu->ROM = *rom;
    ppu->CHRROM = (byte*) malloc(sizeof(byte) * CHRROM_SIZE);
}

void printCHRROM(){
    printMemory(ppu->CHRROM, CHRROM_SIZE);
}