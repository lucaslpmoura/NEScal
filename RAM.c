#include "RAM.h"

RAM InitRAM(){
    RAM ram = (byte*) malloc(sizeof(byte) * RAM_SIZE);
    for(uint16_t i = 0; i < RAM_SIZE; i++){
        ram[i] = 0x00;
    }
    return ram;
}

void PrintRAM(RAM ram){

    for(uint16_t i = 0; i < RAM_SIZE; i++){
        printf("[%04x]:", i);
        for(char j = 0; j < 8; j++){
            printf(" %02x ", ram[i]);
            i++;
        }
        i--;
        printf("\n");
       
    }
}