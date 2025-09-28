#include "RAM.h"

RAM ram = NULL;

void initRAM()
{
    ram = (RAM)malloc(sizeof(RAM) * RAM_SIZE);
    for (uint16_t i = 0; i < RAM_SIZE; i++)
    {
        ram[i] = 0x00;
    }
}

void printRAM()
{

    for (uint16_t i = 0; i < RAM_SIZE; i++)
    {
        
        if (isLineNotZero(i))
        {
            printf("[%04x]:", i);
            for (char j = 0; j < 8; j++)
            {
                printf(" %02x ", ram[i]);
                i++;
            }
            printf("\n");
        }else{
            if(isLineNotZero(i-8)){
                printf("...\n");
            }
            
            i = i+8;
        }
        i--;
        
    }
}

bool isLineNotZero(uint16_t line)
{
    if(line < 0){
        return false;
    }
    for (uint16_t i = line; i < line + 8; i++)
    {
        if (ram[i] != 0x00)
            return true;
    }
    return false;
}

void printStack()
{
    for (uint16_t i = STACK_START_ADDR; i < STACK_START_ADDR + STACK_SIZE; i++)
    {
        printf("[%04x]:", i);
        for (char j = 0; j < 8; j++)
        {
            printf(" %02x ", ram[i]);
            i++;
        }
        i--;
        printf("\n");
    }
}