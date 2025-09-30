#include "PPU.h"

PPU *ppu = NULL;

void initPPU(ROM *rom){
    ppu = (PPU*) malloc(sizeof(PPU));

    ppu->WL = false;
    ppu->tempAddr = 0x0000;

    ppu->transferAddr = 0x0000;
    ppu->vRamAddr = 0x0000;

    ppu->rom = *rom;
    ppu->chrRom = (byte*) malloc(sizeof(byte) * CHRROM_SIZE);
    ppu->vRam = (byte*) malloc(sizeof(byte) * VRAM_SIZE);
    ppu->palleteRam = (byte*) malloc(sizeof(byte) * PALLETE_RAM_SIZE);
}

void writePPU(addr address, byte value){
    switch (address)
    {
    case PPUCTRL:
        break;
    case PPUMASK:
        break;
    case PPUSTATUS:
        break;
    case OAMADDR:
        break;
    case OAMDATA:
        break;
    case PPUSCROLL:
        break;
    case PPUADDR:
        /*
        To set the 14-bit Vram Addres, the CPU needs to write two times. 
        The write latch is responsible for determining if it is the first or the second time writing.
        */
        if(!ppu->WL){
            ppu->tempAddr =  (addr) ((value & 0x3F) << 8);
        }else{
            ppu->vRamAddr = (addr) (ppu->tempAddr | value);
            ppu->transferAddr = ppu->vRamAddr;
        }

        ppu->WL = !ppu->WL;
        break;
    case PPUDATA:
        if(ppu->vRamAddr < PATTERNTABLE_FINAL_ADDR){
            // write to pattern tables
        }else if(ppu->vRamAddr < NAMETABLE_FINAL_ADDR){
            // write to name atables
        }else{
            // write to Pallete Ram
        }
        break;
    
    default:
        break;
    }
}

void printCHRROM(){
    printMemory(ppu->chrRom, CHRROM_SIZE);
}