#include "CPU.h"
#include "MemMapping.h"

#include "RAM.h"
#include "ROM.h"

#include "stdio.h"
#include "stdlib.h"

CPU cpu;

RAM ram;
ROM rom;

FILE *romFile;
long romFileLength;
uint8_t *buffer;

byte* readFileAsBytes();

int main()
{

    ram = InitRAM();
    rom = InitROM();


    Reset();
    return 0;
}

byte Read(addr address)
{
    // RAM
    if ((address >= RAM_INIT_ADDR) && (address < RAM_FINAL_ADDR)){
        return ram[address];
    }

    // RAM Mirrors
    if((address >= RAM_MIRROR_INIT_ADDR) && (address < RAM_MIRROR_FINAL_ADDR)){
        return ram[address % RAM_SIZE];
    }

    //ROM
    if((address >= ROM_INIT_ADDR) && (address < ROM_FINAL_ADDR)){
        return rom[address-ROM_SIZE];
    }

    return 0x00;
}

void Reset(){
    byte* HeaderedRom = readFileAsBytes();

    for(long i = 0; i < romFileLength; i++){
        printf("[%d] - %d\n", i, HeaderedRom[i]);
    }
}


byte* readFileAsBytes(){
    romFile = fopen("test.txt", "rb");
    fseek(romFile, 0, SEEK_END);
    romFileLength = ftell(romFile);
    rewind(romFile);

    buffer = (byte*) malloc(romFileLength * sizeof(byte));
    fread(buffer,1,romFileLength, romFile);

    fclose(romFile);

    return buffer;
}
