#include <stdio.h>
#include "types.h"
#include "ROM.h"

char *romFileName ="";

// Used to create a rom file of the right size (HEADERED_ROM_SIZE)
void createRomFile()
{

    FILE *romFile = fopen(romFileName, "wb");
    if (!romFile)
    {
        perror("Error opening file");
        return NULL;
    }

    // That 0x10 is needed to allocate the header.
    byte *buffer = calloc((HEADERED_ROM_SIZE), sizeof(byte));

    if (!buffer)
    {
        perror("Error alocating memory");
        fclose(romFile);
        return NULL;
    }

    for (addr i = 0; i < (HEADERED_ROM_SIZE); i++)
    {
        buffer[i] = 0xFF;
    }
    fwrite(buffer, HEADERED_ROM_SIZE, 1, romFile);
}