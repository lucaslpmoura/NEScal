#include "PPU.h"

PPU *ppu = NULL;

void initPPU(ROM *rom){
    ppu = (PPU*) malloc(sizeof(PPU));

    ppu->WL = false;
    ppu->tempAddr = 0x0000;

    ppu->transferAddr = 0x0000;

    ppu->vRam = (byte*) malloc(sizeof(byte) * VRAM_SIZE);
    ppu->vRamAddr = 0x0000;
    ppu->vRamInc32Mode = false;

    ppu->rom = *rom;
    ppu->header = (byte*) malloc(sizeof(byte) * HEADER_SIZE);

    ppu->chrData = (byte*) malloc(sizeof(byte) * CHRDATA_SIZE);
    
    ppu->palleteRam = (byte*) malloc(sizeof(byte) * PALLETE_RAM_SIZE);

    ppu->readBuffer = 0x00;
}

byte readPPU(addr address){
    switch (address)
    {
    case PPUSTATUS:
        return 0x80;
    case PPUDATA:
        byte temp = ppu->readBuffer;

        // Reading from the pattern table
        if(ppu->vRamAddr < PATTERNTABLE_FINAL_ADDR){
            ppu->readBuffer = ppu->chrData[ppu->vRamAddr];
        }

        // Reading from the nametable
        else if(ppu->vRamAddr < NAMETABLE_FINAL_ADDR){
            if((ppu->header[HORIZONTAL_MIRRORING_BIT] & 1) == 0){

                // Horizontal Mirroring
                ppu->readBuffer = ppu->vRam[(ppu->vRamAddr & 0x3FF) | (ppu->vRamAddr & 0x800) >> 1];

            }else{

                // Vertical Mirroring
                ppu->readBuffer = ppu->vRam[ppu->vRamAddr & 0x7FF];

            }

        // Reading from Pallete Ram
        }else{
            if((ppu->vRamAddr & 0x03) == 0){
                temp = ppu->palleteRam[ppu->vRamAddr & 0x0F];
            }else{
                temp = ppu->palleteRam[ppu->vRamAddr & 0x1F];
            }
        }

        ppu->vRamAddr += (addr)(ppu->vRamInc32Mode ? 32 : 1);
        ppu->vRamAddr &= 0x3FFF;
        break;
    default:
        return 0x00;
    }
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

        // Pattern Tables
        if(ppu->vRamAddr < PATTERNTABLE_FINAL_ADDR){
            // If there is no pattern table, write thre. Other wise it is read-only.
            if(ppu->header[CHR_DATA_SIZE_BIT] == 0){
                ppu->chrData[ppu->vRamAddr] = value;
            }

        // Name Tables
        }else if(ppu->vRamAddr < NAMETABLE_FINAL_ADDR){
            if((ppu->header[HORIZONTAL_MIRRORING_BIT] & 1) == 0){

                // Horizontal Mirroring
                ppu->vRam[(ppu->vRamAddr & 0x3FF) | (ppu->vRamAddr & 0x800) >> 1] = value;

            }else{

                // Vertical Mirroring
                ppu->vRam[ppu->vRamAddr & 0x7FF] = value;

            }

        // Pallete Ram
        }else{

            // Write to Pallete Ram (mirrors)
            if((ppu->vRamAddr & 3) == 0){
                ppu->palleteRam[ppu->vRamAddr & 0x0F] = value;
            }else{
                ppu->palleteRam[ppu->vRamAddr & 0x1F] = value;
            }

        }

        ppu->vRamAddr += (addr)(ppu->vRamInc32Mode ? 0x20 : 0x01);
        ppu->vRamAddr &= 0x3FFF;
        break;
    
    default:
        break;
    }
}

void printCHRROM(){
    printMemory(ppu->chrData, CHRDATA_SIZE);
}