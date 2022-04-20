#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <stdio.h>

#include "../include/instructionMemory.h"


#define BUFFER_SIZE 32


void loadFromFile(const char* file, InstructionMemory *mem);
int parseInstruction(char* inst);

#endif