#include "emulator/CPU.h"
#include "instructions.h"

#include "PPU.h"

#include "RAM.h"
#include "ROM.h"
#include "MemMapping.h"

#include <stdio.h>
#include <stdlib.h>

#define START -1
#define STEPPING_MODE 0
#define PRINT_STACK 0
#define PRINT_RAM 1
#define PRINT_CHRROM 0

char *romFileName = "./tst/1_Example.nes";

FILE *romFile;
long romFileLength;
uint8_t *buffer;

void run();
void reset();

byte *readFileAsBytes(size_t size, size_t offset);
void copyByteArray(byte *inArray, byte *outArray, uint32_t size, uint32_t offset);

int main(int argv, char **argc)
{
    if (argv == 2)
    {
        romFileName = argc[1];
    }

    initRAM();
    initROM();
    initCPU(&ram, &rom);

    initPPU(&rom);

    printf("Executing: %s\n", romFileName);
    reset();
    run();
    return 0;
}


void run()
{
    if(cpu == NULL){
        perror("CPU not initialized.");
        exit(1);
    }

    if(STEPPING_MODE == 1) printf("Press any key to run.\n");

    printTraceLog(START);
    while (!cpu->HLT)
    {
        if (STEPPING_MODE == 1)
        {
            getchar();
        }

        emulate();
        
        if(PRINT_STACK == 1) {
            printf("\nSTACK: \n");
            printStack();
        }    
    }
    if(PRINT_RAM == 1){
        printf("\n --- RAM ---- \n");
        printf("\n");
        printRAM();
    }
    if(PRINT_CHRROM == 1){
        printf("\n --- CHARACTER ROM ---- \n");
        printCHRROM();
    }
    free(ram);
    free(rom);

    free(cpu);
    free(ppu->CHRROM);
    free(ppu);
}

void reset()
{
 
    cpu->IND = 1;
    cpu->SP = 0xFD;

    byte *HeaderedRom = readFileAsBytes(0xA000, 0);

    if (HeaderedRom == NULL)
    {
        exit(1);
    }

    //byte *Header = (byte *)malloc(0x10 * sizeof(byte));

    // Copies the File to the working ROM
    copyByteArray(HeaderedRom, rom, ROM_SIZE, 0x10);

    // Copies the Character ROM to the PPU CHRROM Struct
    copyByteArray(HeaderedRom, ppu->CHRROM, CHRROM_SIZE, 0x8010);

    byte PCL = read(0xFFFC);
    byte PCH = read(0xFFFD);

    cpu->PC = (addr)((PCH * 0x100) + PCL);
}

byte *readFileAsBytes(size_t size, size_t offset)
{
    FILE *romFile = fopen(romFileName, "rb");
    if (!romFile)
    {
        perror("Error opening file");
        return NULL;
    }

    byte *buffer = calloc(size, sizeof(byte));

    if (!buffer)
    {
        perror("Error alocating memory");
        fclose(romFile);
        return NULL;
    }
    if(offset > 0){
        fseek(romFile, offset, SEEK_SET);
    }

    romFileLength = fread(buffer, 1, size, romFile);

    fclose(romFile);
    return buffer;
}

void copyByteArray(byte *inArray, byte *outArray, uint32_t size, uint32_t offset)
{   
    
    for (size_t i = offset; i < (size + offset); i++)
    {
        outArray[i - offset] = inArray[i];
    }
}





