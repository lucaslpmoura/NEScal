#include "util.h"


void printMemory(byte *mem, size_t size)
{

    for (uint16_t i = 0; i < size; i++)
    {
        
        if (isLineNotZero(mem, i))
        {
            printf("[%04x]:", i);
            for (char j = 0; j < 8; j++)
            {
                printf(" %02x ", mem[i]);
                i++;
            }
            printf("\n");
        }else{
            
            if(isLineNotZero(mem, i-8)){
                printf("...\n");
            }
            
            i = i+8;
        }
        i--;
        
    }
}


    
bool isLineNotZero(byte *mem, uint16_t line)
{
    if(line < 0){
        return false;
    }
    for (uint16_t i = line; i < line + 8; i++)
    {
        if (mem[i] != 0x00)
           return true;
    }
    return false;
}
