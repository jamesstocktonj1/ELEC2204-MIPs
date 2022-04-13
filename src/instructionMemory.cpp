#include "../include/instructionMemory.h"

#include <stdlib.h>
#include <string.h>


InstructionMemory::InstructionMemory() {

    memory = (int *)malloc(INSTRUCTION_MEMORY_SIZE * sizeof(int));
    memset(memory, 0, INSTRUCTION_MEMORY_SIZE);
}

InstructionMemory::~InstructionMemory() {

    free(memory);
}

int InstructionMemory::loadData(int address) {

    return memory[address];
}

void InstructionMemory::writeMemory(int address, int value) {

    memory[address] = value;
}