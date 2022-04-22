#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <cstring>
#include <stdio.h>

#include "../include/instructionMemory.h"


#define BUFFER_SIZE 32

typedef enum {
    IType,
    RType,
    JType
} InstructionType;


typedef struct t_Instruction {
    char *opcode;
    InstructionType instructionType;
    int rs = 1;
    int rt = 2;
    int rd = 3;
    int constant = 4;
} Instruction;


void loadFromFile(const char* file, InstructionMemory *mem);
Instruction getInstruction(char* inst);

InstructionType getType(char* opcode);

int charToInt(char* num);

#endif