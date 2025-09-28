#include "CPU.h"

CPU *cpu = NULL;

void initCPU(RAM *ram, ROM *rom)
{
    cpu = (CPU *)malloc(sizeof(CPU));

    // Pointes
    cpu->PC = 0x0000; // Program Count
    cpu->SP = 0x00; // Stack Pointer

    // Registers
    cpu->A = 0x00;
    cpu->X = 0x00;
    cpu->Y = 0x00;

    // Flags
    cpu->HLT = 0;
    cpu->CRY = 0;
    cpu->ZER = 0;
    cpu->IND = 0;
    cpu->DEC = 0;
    cpu->OVF = 0;
    cpu->NEG = 0;

    cpu->ram = *ram;
    cpu->rom = *rom;

}


void emulate()
{
    byte opcode = read(cpu->PC);
    printTraceLog(opcode);

     
    cpu->PC++;

    int cycles = 0;

 
    switch (opcode)
    {
    // Halt (Unnoficial)
    case HTL:
        cpu->HLT = 1;
        break;
        
    // No Operation
    case NOP:
        cycles = 2;
        break;

    // Break
    case BRK: {
        cpu->PC++;

        push((byte)(cpu->PC >> 8)); // PC High Byte
        push(cpu->PC);              // PC Low Byte
        pushProcessorFlags();

        byte low = read(0xFFFE);
        byte high = read(0xFFFF);

        cpu->PC = (addr)((high * 0x100) + low);
        cycles = 7;
        break;

    }   
        
    // Return from Interrupt
    case RTI: {
        pullProcessorFlags();
        byte low = pull();
        byte high = pull();

        cpu->PC = (addr)((high * 0x100) + low);
        cycles = 6;
        break;
    }
        
    // Load Register Instructions
    // Load X
    case LDX_IM:
        //printf("Loading X with value: %02X", read(cpu->PC));
        load(&cpu->X, read(cpu->PC));

        cycles = 2;
        cpu->PC++;
        break;
    case LDX_ZP:
    {
        load(&cpu->X, read(getZeroPageAddress()));

        cycles = 3;
        break;
    }
    case LDX_AB:
    {
        load(&cpu->X, read(getAbsoluteAddress()));

        cycles = 4;
        break;
    }

    // Load Y
    case LDY_IM:
        load(&cpu->Y, read(cpu->PC));

        cpu->PC++;
        cycles = 2;
        break;
    case LDY_ZP:
    {
        load(&cpu->Y, read(getZeroPageAddress()));

        cycles = 3;
        break;
    }
    case LDY_AB:
    {
        load(&cpu->Y, read(getAbsoluteAddress()));

        cycles = 4;
        break;
    }

    // Load A
    case LDA_IM:
        load(&cpu->A, read(cpu->PC));

        cpu->PC++;
        cycles = 2;
        break;
    case LDA_ZP:
    {
        load(&cpu->A, read(getZeroPageAddress()));
        cycles = 3;
        break;
    }
    case LDA_ZX:
        load(&cpu->A, read(getZeroPageIndexedAddress(cpu->X)));
        cycles = 4;
        break;
    case LDA_AB:
    {
        load(&cpu->A, read(getAbsoluteAddress()));
        cycles = 4;
        break;
    }
    case LDA_AY:
        load(&cpu->A, read(getAbsoluteIndexedAddress(cpu->Y)));
        cycles = 4;
        break;
    case LDA_AX:
        load(&cpu->A, read(getAbsoluteIndexedAddress(cpu->X)));
        cycles = 4;
        break;
    case LDA_IY:
        load(&cpu->A, read(getIndirectIndexedAddressYRegister()));
        cycles = 5;
        break;
    case LDA_IX:
        load(&cpu->A, read(getIndirectIndexedAddressXRegister()));
        cycles = 6;
        break;

    // Store Instructions
    // Register X
    case STX_ZP:
    {
        write(getZeroPageAddress(), cpu->X);
        cycles = 3;
        break;
    }
    case STX_ZY:
        write(getZeroPageIndexedAddress(cpu->Y), cpu->X);
        cycles = 4;
        break;
    case STX_AB:
    {
        write(getAbsoluteAddress(), cpu->X);
        cycles = 4;
        break;
    }
    // Register Y
    case STY_ZP:
    {
        write(getZeroPageAddress(), cpu->Y);
        cycles = 3;
        break;
    }
    case STY_ZX:
        write(getZeroPageIndexedAddress(cpu->X), cpu->X);
        cycles = 4;
        break;
    case STY_AB:
    {
        write(getAbsoluteAddress(), cpu->Y);

        cycles = 4;
        break;
    }
    // Register A
    case STA_ZP:
    {
        write(getZeroPageAddress(), cpu->A);

        cycles = 3;
        break;
    }
    case STA_ZX: {
        write(getZeroPageIndexedAddress(cpu->X), cpu->A);

        cycles = 4;
        break;
    }
    case STA_AB:
    {
        write(getAbsoluteAddress(), cpu->A);

        cycles = 4;
        break;
    }
     case STA_AY:
        write(getAbsoluteIndexedAddress(cpu->Y), cpu->A);

        cycles = 5;
        break;    
    case STA_AX:
        write(getAbsoluteIndexedAddress(cpu->X), cpu->A);

        cycles = 5;
        break;
    case STA_IY:
        write(getIndirectIndexedAddressYRegister(), cpu->A);

        cycles = 6;
        break;
    case STA_IX:
        write(getIndirectIndexedAddressYRegister(), cpu->A);

        cycles = 6;
        break;

    // Branch Instructions
    case BPL:
    {
        cycles = branch(!cpu->NEG);
        break;
    }
    case BMI:
    {
        cycles = branch(cpu->NEG);
        break;
    }

    case BVC:
    {
        cycles = branch(!cpu->OVF);
        break;
    }
    case BVS:
    {
        cycles = branch(cpu->OVF);
        break;
    }

    case BCC:
    {
        cycles = branch(!cpu->CRY);
        break;
    }
    case BCS:
    {
        cycles = branch(cpu->CRY);
        break;
    }

    case BNE:
    {
        cycles = branch(!cpu->ZER);
        break;
    }

    case BEQ:
    {
        cycles = branch(cpu->ZER);
        break;
    }

    // Stack Instrcutions
    case PHA:
        push(cpu->A);
        cycles = 3;
        break;
    case PLA:
        cpu->A = pull();

        cpu->ZER = cpu->A == 0;
        cpu->NEG = cpu->A > 0x7F;

        cycles = 4;
        break;

    case PHP:
    {
        pushProcessorFlags();

        cycles = 3;
        break;
    }
    case PLP:
    {
        pullProcessorFlags();

        cycles = 3;
        break;
    }

    // Jump Instructions
    case JMP_AB:
        byte low = read(cpu->PC);
        cpu->PC++;
        byte high = read(cpu->PC);

        cpu->PC = (addr)(high * 256 + low);
        cycles = 3;
        break;
    case JMP_ID: {
        byte low = read(cpu->PC);
        cpu->PC++;
        byte high = read(cpu->PC);

        cpu->PC = (addr)((read(high) * 256) + read(low));
        cycles = 5;
        break;
    }
        
    case JSR:
    {
        byte low = read(cpu->PC);
        cpu->PC++;
        byte high = read(cpu->PC);

        
        push((byte)(cpu->PC / 256)); // PC High Byte
        push((byte)cpu->PC);       // PC Low Byte

        cpu->PC = (addr)(high * 256 + low);

        cycles = 6;
        break;
    }
    case RTS:
    {
        byte low = pull();
        byte high = pull();

        cpu->PC = (addr)(high * 256 + low);
        cpu->PC++;

        cycles = 6;
        break;
    }

    // Increment / Decrement Instructinos
    case INC_AB:
    {
        addr addr = getAbsoluteAddress();
        byte value = read(addr);
        increment(addr, &value);
        cycles = 6;
        break;
    }
    case INC_ZP:
    {
        addr addr = getZeroPageAddress();
        byte value = read(addr);
        increment(addr, &value);
        cycles = 5;
        break;
    }
    case DEC_AB:
    {
        addr addr = getAbsoluteAddress();
        byte value = read(addr);
        decrement(addr, &value);
        cycles = 6;
        break;
    }
    case DEC_ZP:
    {
        addr addr = getZeroPageAddress();
        byte value = read(addr);
        decrement(addr, &value);
        cycles = 5;
        break;
    }
    case INX:
        increment(-1, &cpu->X);
        cycles = 2;
        break;
    case DEX:
        decrement(-1, &cpu->X);
        cycles = 2;
        break;

    case INY:
        increment(-1, &cpu->Y);
        cycles = 2;
        break;
    case DEY:
        decrement(-1, &cpu->Y);
        cycles = 2;
        break;

        // Transfer Instructions

    case TAX:
        transfer(&cpu->A, &cpu->X, true);
        cycles = 2;
        break;
    case TXA:
        transfer(&cpu->X, &cpu->A, true);
        cycles = 2;
        break;
    case TAY:
        transfer(&cpu->A, &cpu->Y, true);
        cycles = 2;
        break;
    case TYA:
        transfer(&cpu->Y, &cpu->A, true);
        cycles = 2;
        break;
    case TXS:
        /*
        The flags of the 6502 register are not checked on
        SP operations.
        */
        transfer(&cpu->X, &cpu->SP, false);
        cycles = 2;
        break;
    case TSX:
        transfer(&cpu->SP, &cpu->X, true);
        cycles = 2;
        break;

    // Flags Instructions
    case SEC:
        cpu->CRY = 1;
        cycles = 2;
        break;
    case CLC:
        cpu->CRY = 0;
        cycles = 2;
        break;
    case CLV:
        cpu->OVF = 0;
        cycles = 2;
        break;
    case SEI:
        cpu->IND = 1;
        cycles = 2;
        break;
    case CLI:
        cpu->IND = 0;
        cycles = 2;
        break;
    case SED:
        cpu->DEC = 1;
        cycles = 2;
        break;
    case CLD:
        cpu->DEC = 0;

        cycles = 2;
        break;

    // Arithimetic Instructions
    // Add With Carry
    case ADC_IM:
        adc(read(cpu->PC));
        cpu->PC++;
        cycles = 2;
        break;
    case ADC_AB:
        adc(read(getAbsoluteAddress()));
        cycles = 4;
        break;
    case ADC_ZP:
        adc(read(getZeroPageAddress()));
        cycles = 3;
        break;

    // Subtract with Carry
    case SBC_IM:
        sbc(read(cpu->PC));
        cpu->PC++;
        cycles = 2;
        break;
    case SBC_AB:
        sbc(read(getAbsoluteAddress()));
        cycles = 4;
        break;
    case SBC_ZP:
        sbc(read(getZeroPageAddress()));
        cycles = 3;
        break;

    // Arithimetic Shift Left
    case ASL_RA:
        asl(-1, &cpu->A);
        cycles = 2;
        break;
    case ASL_AB:
    {
        addr addr = getAbsoluteAddress();
        byte value = read(addr);
        asl(addr, &value);

        cycles = 6;
        break;
    }
    case ASL_ZP:
    {
        addr addr = getZeroPageAddress();
        byte value = read(addr);
        asl(addr, &value);

        cycles = 5;
        break;
    }

    // Logical Shift Right:
    case LSR_RA:
        lsr(-1, &cpu->A);
        cycles = 2;
        break;
    case LSR_AB:
    {
        addr addr = getAbsoluteAddress();
        byte value = read(addr);
        lsr(addr, &value);

        cycles = 6;
        break;
    }
    case LSR_ZP:
    {
        addr addr = getZeroPageAddress();
        byte value = read(addr);
        lsr(addr, &value);

        cycles = 5;
        break;
    }

    // Rotate Left
    case ROL_RA:
    {
        rol(-1, &cpu->A);
        cycles = 2;
        break;
    }
    case ROL_AB:
    {
        addr addr = getAbsoluteAddress();
        byte value = read(addr);
        rol(addr, &value);

        cycles = 6;
        break;
    }
    case ROL_ZP:
    {
        addr addr = getZeroPageAddress();
        byte value = read(addr);
        rol(addr, &value);

        cycles = 5;
        break;
    }

    // Rotate Right
    case ROR_RA:
    {
        ror(-1, &cpu->A);
        cycles = 2;
        break;
    }
    case ROR_AB:
    {
        addr addr = getAbsoluteAddress();
        byte value = read(addr);
        ror(addr, &value);

        cycles = 6;
        break;
    }
    case ROR_ZP:
    {
        addr addr = getZeroPageAddress();
        byte value = read(addr);
        ror(addr, &value);

        cycles = 5;
        break;
    }

    // Bitwise operations
    // OR with register A
    case ORA_IM:
        ora(read(cpu->PC));
        cpu->PC++;
        cycles = 2;
        break;
    case ORA_AB:
        ora(read(getAbsoluteAddress()));
        cycles = 4; // TODO: Special Case
        break;
    case ORA_ZP:
        ora(read(getZeroPageAddress()));
        cycles = 3;
        break;

    // AND with register A
    case AND_IM:
        and(read(cpu->PC));
        cpu->PC++;
        cycles = 2;
        break;
    case AND_AB:
        and(read(getAbsoluteAddress()));
        cycles = 4; // TODO: Special Case
        break;
    case AND_ZP:
        and(read(getZeroPageAddress()));
        cycles = 3;
        break;

    // XOR with register A
    case EOR_IM:
        eor(read(cpu->PC));
        cpu->PC++;
        cycles = 2;
        break;
    case EOR_AB:
        eor(read(getAbsoluteAddress()));
        cycles = 4; // TODO: Special Case
        break;
    case EOR_ZP:
        eor(read(getZeroPageAddress()));
        cycles = 3;
        break;

    // Compare Instructions
    // Compare Accumulator
    case CMP_IM:
        cmp(read(cpu->PC), &cpu->A);
        cpu->PC++;
        cycles = 2;
        break;
    case CMP_AB:
        cmp(read(getAbsoluteAddress()), &cpu->A);
        cycles = 4;
        break;
    case CMP_ZP:
        cmp(read(getZeroPageAddress()), &cpu->A);
        cycles = 3;
        break;

    // Compare Register X
    case CPX_IM:
        cmp(read(cpu->PC), &cpu->X);
        cpu->PC++;
        cycles = 2;
        break;
    case CPX_AB:
        cmp(read(getAbsoluteAddress()), &cpu->X);
        cycles = 2;
        break;
    case CPX_ZP:
        cmp(read(getZeroPageAddress()), &cpu->X);
        cycles = 3;
        break;

    // Compare Register Y
    case CPY_IM:
        cmp(read(cpu->PC), &cpu->Y);
        cpu->PC++;
        cycles = 2;
        break;
    case CPY_AB:
        cmp(read(getAbsoluteAddress()), &cpu->Y);
        cycles = 2;
        break;
    case CPY_ZP:
        cmp(read(getZeroPageAddress()), &cpu->X);
        cycles = 3;
        break;

    // Bit Compare
    case BIT_AB:
        bit(read(getAbsoluteAddress()));
        cycles = 4;
        break;
    case BIT_ZP:
        bit(read(getZeroPageAddress()));
        cycles = 3;
        break;

    default:
        printf("Unknown instruction: %x.\n", opcode);
        // exit(1);
        break;
    }

}



byte read(addr address)
{
    // RAM
    if ((address >= RAM_INIT_ADDR) && (address < RAM_FINAL_ADDR))
    {
        return cpu->ram[address];
    }

    // RAM Mirrors
    if ((address >= RAM_MIRROR_INIT_ADDR) && (address < RAM_MIRROR_FINAL_ADDR))
    {
        return cpu->ram[address % RAM_SIZE];
    }

    // ROM
    if ((address >= ROM_INIT_ADDR) && (address < ROM_FINAL_ADDR))
    {
        return cpu->rom[address - ROM_SIZE];
    }

    return 0x00;
}

void write(addr address, byte value)
{
    // RAM
    if ((address >= RAM_INIT_ADDR) && (address < RAM_FINAL_ADDR))
    {
        cpu->ram[address] = value;
    }

    // RAM Mirrors
    if ((address >= RAM_MIRROR_INIT_ADDR) && (address < RAM_MIRROR_FINAL_ADDR))
    {
        cpu->ram[address % RAM_SIZE] = value;
    }
}

void load(byte *reg, byte value)
{
    *reg = value;

    cpu->ZER = *reg == 0;
    cpu->NEG = *reg > 0x7F;
}



void push(byte value)
{
    addr addr = (STACK_START_ADDR + cpu->SP);
    write(addr, value);
    cpu->SP--;
}

void pushProcessorFlags()
{
    byte value = 0;
    value += (byte)(cpu->CRY ? 0x01 : 0);
    value += (byte)(cpu->ZER ? 0x02 : 0);
    value += (byte)(cpu->IND ? 0x04 : 0);
    value += (byte)(cpu->DEC ? 0x08 : 0);
    value += 0x10;
    value += 0x20;
    value += (byte)(cpu->OVF ? 0x40 : 0);
    value += (byte)(cpu->NEG ? 0x80 : 0);
    push(value);
}

byte pull()
{
    cpu->SP++;
    addr addr = (STACK_START_ADDR + cpu->SP);
    byte value = read(addr);
    return value;
}

void pullProcessorFlags()
{
    byte value = pull();

    cpu->CRY = (value & 0x01) != 0;
    cpu->ZER = (value & 0x02) != 0;
    cpu->IND = (value & 0x04) != 0;
    cpu->DEC = (value & 0x08) != 0;
    cpu->OVF = (value & 0x40) != 0;
    cpu->NEG = (value & 0x80) != 0;
}



int branch(bool flag)
{
    int cycles = 0;
    byte temp = read(cpu->PC);
    cpu->PC++;

    if (flag)
    {
        int signedVal = convertToSignedVal(temp);
        printf("Branching from %02X with offset (%02X/%d)", cpu->PC, temp, signedVal);
        cpu->PC += (addr) signedVal;
        printf(" to %02X\n", cpu->PC);
        cycles = 3;
    }
    else
    {
        cycles = 2;
    }
    return cycles;
}
void increment(addr addr, byte *value)
{
    
    *value = *value + 0x01;

    if (addr != -1)
    {
        write(addr, *value);
    }
    cpu->NEG = *value > 0x7F;
    cpu->ZER = *value == 0;
}

void decrement(addr addr, byte *value)
{   
    *value = *value - 1;
    if (addr != -1)
    {
        write(addr, *value);
    }
    cpu->NEG = *value > 0x7F;
    cpu->ZER = *value == 0;
}

void transfer(byte *sourceRegister, byte *destinationRegister, bool changeFlags)
{
    *destinationRegister = *sourceRegister;

    if (changeFlags)
    {
        cpu->ZER = *destinationRegister == 0;
        cpu->NEG = *destinationRegister > 0x7F;
    }
}


void adc(byte value)
{
    int sum = value + cpu->A + (cpu->CRY ? 1 : 0);

    cpu->OVF = (~(cpu->A ^ value) & (cpu->A ^ sum) & 0x80) != 0;

    cpu->A = (byte)sum;

    cpu->CRY = sum > 0xFF;
    cpu->NEG = cpu->A > 0x7F;
    cpu->ZER = cpu->A == 0;
}

void sbc(byte value)
{
    int sub = cpu->A - value - (cpu->CRY ? 0 : 1);

    cpu->OVF = ((cpu->A ^ value) & (cpu->A ^ sub) & 0x80) != 0;

    cpu->A = (byte)sub;

    cpu->CRY = sub >= 0;
    cpu->NEG = cpu->A > 0x7F;
    cpu->ZER = cpu->A == 0;
}

void asl(addr addr, byte *value)
{
    printf("Executing: ASL on value: %x\n", *value);
    cpu->CRY = *value > 0x7F;

    *value <<= 1;

    cpu->ZER = *value == 0;
    cpu->NEG = *value > 0x7F;

    if (addr != -1)
    {
        write(addr, *value);
    }
}

void lsr(addr addr, byte *value)
{
    printf("Executing: LSR on value: %x\n", *value);
    cpu->CRY = *value % 2 != 0; // Shifts the 0th bit to the carry flag.

    *value >>= 1;

    cpu->ZER = *value == 0;
    cpu->NEG = *value > 0x7F;

    if (addr != -1)
    {
        write(addr, *value);
    }
}

void rol(addr addr, byte *value)
{
    printf("Executing: ROL on value: %x\n", *value);

    bool newCarry = *value > 0x7F;

    *value <<= 1;
    if (cpu->CRY)
    {
        *value |= 0b00000001;
    }

    cpu->ZER = *value == 0;
    cpu->NEG = *value > 0x7F;
    cpu->CRY = newCarry;

    if (addr != -1)
    {
        write(addr, *value);
    }
}

void ror(addr addr, byte *value)
{
    printf("Exectuing ROR on value: %x\n", *value);

    bool newCarry = *value > 0x7F;

    *value >>= 1;
    if (cpu->CRY)
    {
        *value |= 0b10000000;
    }

    cpu->ZER = *value == 0;
    cpu->NEG = *value > 0x7F;
    cpu->CRY = newCarry;

    if (addr != -1)
    {
        write(addr, *value);
    }
}




void ora(byte value)
{
    cpu->A |= value;

    cpu->NEG = cpu->A > 0x7F;
    cpu->ZER = cpu->A == 0;
}

void and(byte value)
{
    cpu->A &= value;

    cpu->NEG = cpu->A > 0x7F;
    cpu->ZER = cpu->A == 0;
}

void eor(byte value)
{
    cpu->A ^= value;

    cpu->NEG = cpu->A > 0x7F;
    cpu->ZER = cpu->A == 0;
}

void cmp(byte value, byte *reg)
{
    cpu->CRY = value <= *reg;
    cpu->NEG = (byte)(*reg - value) > 127;
    cpu->ZER = value == *reg;
}

void bit(byte value)
{
    cpu->OVF = (value & 0x40) != 0;
    cpu->NEG = (value & 0x80) != 0;
    cpu->ZER = (value & cpu->A) == 0;
}

int convertToSignedVal(byte num)
{
    int signedVal = num > 0x7F ? num - 256 : num;
    return signedVal;
}

addr getAbsoluteAddress()
{
    addr absAddr = read(cpu->PC);
    cpu->PC++;
    absAddr = (addr)(read(cpu->PC) << 8 | absAddr);
    cpu->PC++;
    return absAddr;
}

addr getAbsoluteIndexedAddress(byte reg){
    return getAbsoluteAddress() + reg;
}


addr getZeroPageAddress()
{
    byte temp = read(cpu->PC);
    cpu->PC++;
    return temp;
}


/*
As this function can only index the zero page (0000 - 00FF)
we need to "wrap arround" the offset
*/
addr getZeroPageIndexedAddress(byte reg){
    return (getZeroPageAddress() + reg) % (ROM_INIT_ADDR + 0xFF); 
}

addr getIndirectIndexedAddressYRegister(){
    addr indAddr = read(cpu->PC);
    cpu->PC++;
    byte temp = (byte) indAddr;
    indAddr = read(temp);
    temp++;
    indAddr = (addr)(read(temp) << 8 | indAddr);
    indAddr += cpu->Y;
    return indAddr;
}

addr getIndirectIndexedAddressXRegister(){
    addr indAddr = (addr) (read(cpu->PC) + cpu->X);
    cpu->PC++;
    byte temp = (byte) indAddr;
    indAddr = read(temp);
    temp++;
    indAddr = (addr)(read(temp) << 8 | indAddr);
    indAddr += cpu->Y;
    return indAddr;
}


void printTraceLog(int opcode){
    printf("$%04x \t %02x \t %s \t\t A: %02X X: %02X Y: %02X, SP: %02X, Flags: %s%s%s%s%s%s%s"
    ,cpu->PC
    ,opcode
    ,getOpcodeMnemonic(opcode)
    ,cpu->A
    ,cpu->X
    ,cpu->Y
    ,cpu->SP
    ,cpu->HLT == 1 ? "H" : "h"
    ,cpu->CRY == 1 ? "C" : "c"
    ,cpu->ZER == 1 ? "Z" : "z"
    ,cpu->IND == 1 ? "I" : "i"
    ,cpu->DEC == 1 ? "D" : "d"
    ,cpu->OVF == 1 ? "O" : "o"
    ,cpu->NEG == 1 ? "N" : "n"
);
    printf("\n");

    //printf("\n");
}