#include "CPU.h"

CPU *cpu = NULL;

void initCPU(RAM *ram, ROM *rom, PPU *ppu)
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
    cpu->ppu = ppu;

    cpu->NMI = false;
    cpu->NMILevel = false;

}


void emulateCPU()
{
    bool prevNMILevel = cpu->NMILevel;
    cpu->NMILevel = ppu->enableNMI && ppu->vBlank;
    if(!prevNMILevel && cpu->NMILevel){
        cpu->NMI = true;
    }

    byte opcode;
    
    /*
        If there is no NMI, read the opcode and increment PC.
        If the IS an NMI, force opcode 0x00.
    */
    if(!cpu->NMI){
        readCPU(cpu->PC);
        printTraceLog(opcode);
        cpu->PC++;
    }else{
        opcode = 0x00;
    }
    
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

    // Break and NMI
    case BRK_NMI: {
        if(!cpu->NMI) cpu->PC++;

        push((byte)(cpu->PC >> 8)); // PC High Byte
        push(cpu->PC);              // PC Low Byte
        pushProcessorFlags();

        addr low_addr = cpu->NMI ? 0xFFFA : 0xFFFE;
        addr high_addr = cpu->NMI ? 0xFFFB : 0xFFFF;

        byte low = readCPU(low_addr);
        byte high = readCPU(high_addr);

        cpu->PC = (addr)((high * 0x100) + low);

        cpu->NMI = false;
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
        //printf("Loading X with value: %02X", readCPU(cpu->PC));
        load(&cpu->X, readCPU(cpu->PC));

        cycles = 2;
        cpu->PC++;
        break;
    case LDX_ZP:
    {
        load(&cpu->X, readCPU(getZeroPageAddress()));

        cycles = 3;
        break;
    }
    case LDX_ZY:
        load(&cpu->X, readCPU(getZeroPageIndexedAddress(cpu->Y)));

        cycles = 4;
        break;
    case LDX_AB:
    {
        load(&cpu->X, readCPU(getAbsoluteAddress()));

        cycles = 4;
        break;
    }
    case LDX_AY:
        load(&cpu->X, readCPU(getAbsoluteIndexedAddress(cpu->Y)));

        cycles = 4;
        break;

    // Load Y
    case LDY_IM:
        load(&cpu->Y, readCPU(cpu->PC));

        cpu->PC++;
        cycles = 2;
        break;
    case LDY_ZP:
    {
        load(&cpu->Y, readCPU(getZeroPageAddress()));

        cycles = 3;
        break;
    }
    case LDY_ZX:
    {
        load(&cpu->Y, readCPU(getZeroPageIndexedAddress(cpu->X)));

        cycles = 3;
        break;
    }
    case LDY_AB:
    {
        load(&cpu->Y, readCPU(getAbsoluteAddress()));

        cycles = 4;
        break;
    }
    case LDY_AX:
    {
        load(&cpu->Y, readCPU(getAbsoluteIndexedAddress(cpu->X)));

        cycles = 4;
        break;
    }


    // Load A
    case LDA_IM:
        load(&cpu->A, readCPU(cpu->PC));

        cpu->PC++;
        cycles = 2;
        break;
    case LDA_ZP:
    {
        load(&cpu->A, readCPU(getZeroPageAddress()));
        cycles = 3;
        break;
    }
    case LDA_ZX:
        load(&cpu->A, readCPU(getZeroPageIndexedAddress(cpu->X)));
        cycles = 4;
        break;
    case LDA_AB:
    {
        load(&cpu->A, readCPU(getAbsoluteAddress()));
        cycles = 4;
        break;
    }
    case LDA_AY:
        load(&cpu->A, readCPU(getAbsoluteIndexedAddress(cpu->Y)));
        cycles = 4;
        break;
    case LDA_AX:
        load(&cpu->A, readCPU(getAbsoluteIndexedAddress(cpu->X)));
        cycles = 4;
        break;
    case LDA_IY:
        load(&cpu->A, readCPU(getIndirectIndexedAddressYRegister()));
        cycles = 5;
        break;
    case LDA_IX:
        load(&cpu->A, readCPU(getIndirectIndexedAddressXRegister()));
        cycles = 6;
        break;

    // Store Instructions
    // Register X
    case STX_ZP:
    {
        writeCPU(getZeroPageAddress(), cpu->X);
        cycles = 3;
        break;
    }
    case STX_ZY:
        writeCPU(getZeroPageIndexedAddress(cpu->Y), cpu->X);
        cycles = 4;
        break;
    case STX_AB:
    {
        writeCPU(getAbsoluteAddress(), cpu->X);
        cycles = 4;
        break;
    }
    // Register Y
    case STY_ZP:
    {
        writeCPU(getZeroPageAddress(), cpu->Y);
        cycles = 3;
        break;
    }
    case STY_ZX:
        writeCPU(getZeroPageIndexedAddress(cpu->X), cpu->X);
        cycles = 4;
        break;
    case STY_AB:
    {
        writeCPU(getAbsoluteAddress(), cpu->Y);

        cycles = 4;
        break;
    }
    // Register A
    case STA_ZP:
    {
        writeCPU(getZeroPageAddress(), cpu->A);

        cycles = 3;
        break;
    }
    case STA_ZX: {
        writeCPU(getZeroPageIndexedAddress(cpu->X), cpu->A);

        cycles = 4;
        break;
    }
    case STA_AB:
    {
        writeCPU(getAbsoluteAddress(), cpu->A);

        cycles = 4;
        break;
    }
     case STA_AY:
        writeCPU(getAbsoluteIndexedAddress(cpu->Y), cpu->A);

        cycles = 5;
        break;    
    case STA_AX:
        writeCPU(getAbsoluteIndexedAddress(cpu->X), cpu->A);

        cycles = 5;
        break;
    case STA_IY:
        writeCPU(getIndirectIndexedAddressYRegister(), cpu->A);

        cycles = 6;
        break;
    case STA_IX:
        writeCPU(getIndirectIndexedAddressXRegister(), cpu->A);

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
        byte low = readCPU(cpu->PC);
        cpu->PC++;
        byte high = readCPU(cpu->PC);

        cpu->PC = (addr)(high * 256 + low);
        cycles = 3;
        break;
    case JMP_ID: {
        byte low = readCPU(cpu->PC);
        cpu->PC++;
        byte high = readCPU(cpu->PC);  
        
        addr efAddr = high * 256 + low;

        cpu->PC = (addr)(readCPU(efAddr + 1) * 256 + readCPU(efAddr));
        cycles = 5;
        break;
    }
        
    case JSR:
    {
        byte low = readCPU(cpu->PC);
        cpu->PC++;
        byte high = readCPU(cpu->PC);

        
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
        byte value = readCPU(addr);
        increment(addr, &value);
        cycles = 6;
        break;
    }
    case INC_AX: {
        addr addr = getAbsoluteIndexedAddress(cpu->X);
        byte value = readCPU(addr);
        increment(addr, &value);
        cycles = 7;
        break;
    }
    case INC_ZP:
    {
        addr addr = getZeroPageAddress();
        byte value = readCPU(addr);
        increment(addr, &value);
        cycles = 5;
        break;
    }
    case INC_ZX:
    {
        addr addr = getZeroPageIndexedAddress(cpu->X);
        byte value = readCPU(addr);
        increment(addr, &value);
        cycles = 6;
        break;
    }

    // Decrement
    case DEC_AB:
    {
        addr addr = getAbsoluteAddress();
        byte value = readCPU(addr);
        decrement(addr, &value);
        cycles = 6;
        break;
    }
     case DEC_AX:
    {
        addr addr = getAbsoluteIndexedAddress(cpu->X);
        byte value = readCPU(addr);
        decrement(addr, &value);
        cycles = 7;
        break;
    }
    case DEC_ZP:
    {
        addr addr = getZeroPageAddress();
        byte value = readCPU(addr);
        decrement(addr, &value);
        cycles = 5;
        break;
    }
    case DEC_ZX:
    {
        addr addr = getZeroPageIndexedAddress(cpu->X);
        byte value = readCPU(addr);
        decrement(addr, &value);
        cycles = 6;
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
        adc(readCPU(cpu->PC));
        cpu->PC++;
        cycles = 2;
        break;
    case ADC_AB:
        adc(readCPU(getAbsoluteAddress()));
        cycles = 4;
        break;
    case ADC_AY:
        adc(readCPU(getAbsoluteIndexedAddress(cpu->Y)));
        cycles = 4;
        break;
    case ADC_AX:
        adc(readCPU(getAbsoluteIndexedAddress(cpu->X)));
        cycles = 4;
        break;
    case ADC_ZP:
        adc(readCPU(getZeroPageAddress()));
        cycles = 3;
        break;
    case ADC_ZX:
        adc(readCPU(getZeroPageIndexedAddress(cpu->X)));
        cycles = 4;
        break;
    case ADC_IY:
        adc(readCPU(getIndirectIndexedAddressYRegister()));
        cycles = 5;
        break;
    case ADC_IX:
        adc(readCPU(getIndirectIndexedAddressXRegister()));
        cycles = 6;
        break;

    // Subtract with Carry
    case SBC_IM:
        sbc(readCPU(cpu->PC));
        cpu->PC++;
        cycles = 2;
        break;
    case SBC_AB:
        sbc(readCPU(getAbsoluteAddress()));
        cycles = 4;
        break;
    case SBC_AX:
        sbc(readCPU(getAbsoluteIndexedAddress(cpu->X)));
        cycles = 4;
        break;
    case SBC_AY:
        sbc(readCPU(getAbsoluteIndexedAddress(cpu->Y)));
        cycles = 4;
        break;
    case SBC_ZP:
        sbc(readCPU(getZeroPageAddress()));
        cycles = 3;
        break;
    case SBC_ZX:
        sbc(readCPU(getZeroPageIndexedAddress(cpu->X)));
        cycles = 4;
        break;
    case SBC_IX:
        sbc(readCPU(getIndirectIndexedAddressXRegister()));
        cycles = 6;
        break;
    case SBC_IY:
        sbc(readCPU(getIndirectIndexedAddressYRegister()));
        cycles = 6;
        break;    

    // Arithimetic Shift Left
    case ASL_RA:
        asl(-1, &cpu->A);
        cycles = 2;
        break;
    case ASL_AB:
    {
        addr addr = getAbsoluteAddress();
        byte value = readCPU(addr);
        asl(addr, &value);

        cycles = 6;
        break;
    }
    case ASL_AX:
    {
        addr addr = getAbsoluteIndexedAddress(cpu->X);
        byte value = readCPU(addr);
        asl(addr, &value);

        cycles = 7;
        break;
    }
    case ASL_ZP:
    {
        addr addr = getZeroPageAddress();
        byte value = readCPU(addr);
        asl(addr, &value);

        cycles = 5;
        break;
    }
    case ASL_ZX:
    {
        addr addr = getZeroPageIndexedAddress(cpu->X);
        byte value = readCPU(addr);
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
        byte value = readCPU(addr);
        lsr(addr, &value);

        cycles = 6;
        break;
    }
    case LSR_AX:
    {
        addr addr = getAbsoluteIndexedAddress(cpu->X);
        byte value = readCPU(addr);
        lsr(addr, &value);

        cycles = 7;
        break;
    }
    case LSR_ZP:
    {
        addr addr = getZeroPageAddress();
        byte value = readCPU(addr);
        lsr(addr, &value);

        cycles = 5;
        break;
    }
    case LSR_ZX:
    {
        addr addr = getZeroPageIndexedAddress(cpu->X);
        byte value = readCPU(addr);
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
        byte value = readCPU(addr);
        rol(addr, &value);

        cycles = 6;
        break;
    }
    case ROL_AX:
    {
        addr addr = getAbsoluteIndexedAddress(cpu->X);
        byte value = readCPU(addr);
        rol(addr, &value);

        cycles = 7;
        break;
    }
    case ROL_ZP:
    {
        addr addr = getZeroPageAddress();
        byte value = readCPU(addr);
        rol(addr, &value);

        cycles = 5;
        break;
    }
    case ROL_ZX:
    {
        addr addr = getZeroPageIndexedAddress(cpu->X);
        byte value = readCPU(addr);
        rol(addr, &value);

        cycles = 6;
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
        byte value = readCPU(addr);
        ror(addr, &value);

        cycles = 6;
        break;
    }
    case ROR_AX:
    {
        addr addr = getAbsoluteIndexedAddress(cpu->X);
        byte value = readCPU(addr);
        ror(addr, &value);

        cycles = 7;
        break;
    }
    case ROR_ZP:
    {
        addr addr = getZeroPageAddress();
        byte value = readCPU(addr);
        ror(addr, &value);

        cycles = 5;
        break;
    }
    case ROR_ZX:
    {
        addr addr = getZeroPageIndexedAddress(cpu->X);
        byte value = readCPU(addr);
        ror(addr, &value);

        cycles = 6;
        break;
    }

    // Bitwise operations
    // OR with register A
    case ORA_IM:
        ora(readCPU(cpu->PC));
        cpu->PC++;
        cycles = 2;
        break;
    case ORA_AB:
        ora(readCPU(getAbsoluteAddress()));
        cycles = 4; // TODO: Special Case
        break;
    case ORA_AX:
        ora(readCPU(getAbsoluteIndexedAddress(cpu->X)));
        cycles = 4; // TODO: Special Case
        break;
    case ORA_AY:
        ora(readCPU(getAbsoluteIndexedAddress(cpu->Y)));
        cycles = 4; // TODO: Special Case
        break;
    case ORA_ZP:
        ora(readCPU(getZeroPageAddress()));
        cycles = 3;
        break;
    case ORA_ZX:
        ora(readCPU(getZeroPageIndexedAddress(cpu->X)));
        cycles = 4; // TODO: Special Case
        break;
    case ORA_IX:
        ora(readCPU(getIndirectIndexedAddressXRegister()));
        cycles = 6;
        break;
    case ORA_IY:
        ora(readCPU(getIndirectIndexedAddressYRegister()));
        cycles = 5;
        break;

    // AND with register A
    case AND_IM:
        and(readCPU(cpu->PC));
        cpu->PC++;
        cycles = 2;
        break;
    case AND_AB:
        and(readCPU(getAbsoluteAddress()));
        cycles = 4; // TODO: Special Case
        break;
    case AND_AY:
        and(readCPU(getAbsoluteIndexedAddress(cpu->Y)));
        cycles = 4; 
        break;
    case AND_AX:
        and(readCPU(getAbsoluteIndexedAddress(cpu->X)));
        cycles = 4; 
        break;
    case AND_ZP:
        and(readCPU(getZeroPageAddress()));
        cycles = 3;
        break;
    case AND_ZX:
        and(readCPU(getZeroPageIndexedAddress(cpu->X)));
        cycles = 3;
        break;
    case AND_IX:
        and(readCPU(getIndirectIndexedAddressXRegister()));
        cycles = 6;
        break;
    case AND_IY:
        and(readCPU(getIndirectIndexedAddressYRegister()));
        cycles = 5;
        break;

    // XOR with register A
    case EOR_IM:
        eor(readCPU(cpu->PC));
        cpu->PC++;
        cycles = 2;
        break;
    case EOR_AB:
        eor(readCPU(getAbsoluteAddress()));
        cycles = 4; 
        break;
    case EOR_AX:
        eor(readCPU(getAbsoluteIndexedAddress(cpu->X)));
        cycles = 4; // TODO: Special Case
        break;
    case EOR_AY:
        eor(readCPU(getAbsoluteIndexedAddress(cpu->Y)));
        cycles = 4; // TODO: Special Case
        break;
    case EOR_ZP:
        eor(readCPU(getZeroPageAddress()));
        cycles = 3;
        break;
    case EOR_ZX:
        eor(readCPU(getZeroPageIndexedAddress(cpu->X)));
        cycles = 4;
        break;
    case EOR_IX:
        eor(readCPU(getIndirectIndexedAddressXRegister()));
        cycles = 6;
        break;
    case EOR_IY:
        eor(readCPU(getIndirectIndexedAddressYRegister()));
        cycles = 6;
        break;

    // Compare Instructions
    // Compare Accumulator
    case CMP_IM:
        cmp(readCPU(cpu->PC), &cpu->A);
        cpu->PC++;
        cycles = 2;
        break;
    case CMP_AB:
        cmp(readCPU(getAbsoluteAddress()), &cpu->A);
        cycles = 4;
        break;
    case CMP_AX:
        cmp(readCPU(getAbsoluteIndexedAddress(cpu->X)), &cpu->A);
        cycles = 4;
        break;
    case CMP_AY:
        cmp(readCPU(getAbsoluteIndexedAddress(cpu->Y)), &cpu->A);
        cycles = 4;
        break;
    case CMP_ZP:
        cmp(readCPU(getZeroPageAddress()), &cpu->A);
        cycles = 3;
        break;
    case CMP_ZX:
        cmp(readCPU(getZeroPageIndexedAddress(cpu->X)), &cpu->A);
        cycles = 4;
        break;
    case CMP_IX:
        cmp(readCPU(getIndirectIndexedAddressXRegister()), &cpu->A);
        cycles = 5;
        break;
    case CMP_IY:
        cmp(readCPU(getIndirectIndexedAddressYRegister()), &cpu->A);
        cycles = 5;
        break;
    

    // Compare Register X
    case CPX_IM:
        cmp(readCPU(cpu->PC), &cpu->X);
        cpu->PC++;
        cycles = 2;
        break;
    case CPX_AB:
        cmp(readCPU(getAbsoluteAddress()), &cpu->X);
        cycles = 2;
        break;
    case CPX_ZP:
        cmp(readCPU(getZeroPageAddress()), &cpu->X);
        cycles = 3;
        break;

    // Compare Register Y
    case CPY_IM:
        cmp(readCPU(cpu->PC), &cpu->Y);
        cpu->PC++;
        cycles = 2;
        break;
    case CPY_AB:
        cmp(readCPU(getAbsoluteAddress()), &cpu->Y);
        cycles = 2;
        break;
    case CPY_ZP:
        cmp(readCPU(getZeroPageAddress()), &cpu->X);
        cycles = 3;
        break;

    // Bit Compare
    case BIT_AB:
        bit(readCPU(getAbsoluteAddress()));
        cycles = 4;
        break;
    case BIT_ZP:
        bit(readCPU(getZeroPageAddress()));
        cycles = 3;
        break;

    default:
        printf("Unknown instruction: %x.\n", opcode);
        // exit(1);
        break;
    }

    while(cycles > 0){
        cycles--;
        emulatePPU();
        emulatePPU();
        emulatePPU();
    }

}

void resetCPU(){
    cpu->IND = 1;
    cpu->SP = 0xFD;

    byte PCL = readCPU(0xFFFC);
    byte PCH = readCPU(0xFFFD);

    cpu->PC = (addr)((PCH * 0x100) + PCL);
}



byte readCPU(addr address)
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

    if((address >= PPU_REGISTERS_START_ADDR) && (address < PPU_REGISTERS_FINAL_ADDR)){
        address &= 0x2007;
        return readPPU(address);
    }

    // ROM
    if ((address >= ROM_INIT_ADDR) && (address < ROM_FINAL_ADDR))
    {
        return cpu->rom[address - ROM_SIZE];
    }

    return 0x00;
}

void writeCPU(addr address, byte value)
{   
    printf("Writing %02X to address: %04X\n", value, address);
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

    if((address >= PPU_REGISTERS_START_ADDR) && (address < PPU_REGISTERS_FINAL_ADDR)){
        /*
        There are only 8 PPU register, so every 8 bytes
        is a mirror.
        */
        address &= 0x2007;
        writePPU(address, value); 
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
    writeCPU(addr, value);
    cpu->SP--;
}

void pushProcessorFlags()
{
    byte value = 0;
    value += (byte)(cpu->CRY ? 0x01 : 0);
    value += (byte)(cpu->ZER ? 0x02 : 0);
    value += (byte)(cpu->IND ? 0x04 : 0);
    value += (byte)(cpu->DEC ? 0x08 : 0);
    value += (byte)(!cpu->NMI ? 0x10 : 0);
    value += 0x20;
    value += (byte)(cpu->OVF ? 0x40 : 0);
    value += (byte)(cpu->NEG ? 0x80 : 0);
    push(value);
}

byte pull()
{
    cpu->SP++;
    addr addr = (STACK_START_ADDR + cpu->SP);
    byte value = readCPU(addr);
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
    byte temp = readCPU(cpu->PC);
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
        writeCPU(addr, *value);
    }
    cpu->NEG = *value > 0x7F;
    cpu->ZER = *value == 0;
}

void decrement(addr addr, byte *value)
{   
    *value = *value - 1;
    if (addr != -1)
    {
        writeCPU(addr, *value);
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
        writeCPU(addr, *value);
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
        writeCPU(addr, *value);
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
        writeCPU(addr, *value);
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
        writeCPU(addr, *value);
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
    addr absAddr = readCPU(cpu->PC);
    cpu->PC++;
    absAddr = (addr)(readCPU(cpu->PC) << 8 | absAddr);
    cpu->PC++;
    return absAddr;
}

addr getAbsoluteIndexedAddress(byte reg){
    return getAbsoluteAddress() + reg;
}


addr getZeroPageAddress()
{
    byte temp = readCPU(cpu->PC);
    cpu->PC++;
    return temp;
}


/*
As this function can only index the zero page (0000 - 00FF)
we need to "wrap arround" the offset
*/
addr getZeroPageIndexedAddress(byte reg){
    return (getZeroPageAddress() + reg) % (RAM_INIT_ADDR + 0xFF); 
}

addr getIndirectIndexedAddressYRegister(){
    addr indAddr = readCPU(cpu->PC);
    cpu->PC++;
    byte temp = (byte) indAddr;
    indAddr = readCPU(temp);
    temp++;
    indAddr = (addr)(readCPU(temp) << 8 | indAddr);
    indAddr += cpu->Y;
    return indAddr;
}

addr getIndirectIndexedAddressXRegister(){
    addr indAddr = (addr) (readCPU(cpu->PC) + cpu->X);
    cpu->PC++;
    byte temp = (byte) indAddr;
    indAddr = readCPU(temp);
    temp++;
    indAddr = (addr)(readCPU(temp) << 8 | indAddr);
    return indAddr;
}


void printTraceLog(int opcode){
    printf("$%04x \t %02x \t %6s \t %02X  %02X \t A: %02X X: %02X Y: %02X, SP: %02X, Flags: %s%s%s%s%s%s%s"
    ,cpu->PC
    ,opcode
    ,getOpcodeMnemonic(opcode)
    ,readCPU(cpu->PC + 1)
    ,readCPU(cpu->PC + 2)
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