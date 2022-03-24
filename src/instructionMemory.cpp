#include "../include/instructionMemory.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>


InstructionMemory::InstructionMemory() {

    memory = (int *)malloc(INSTRUCTION_MEMORY_SIZE);
}

InstructionMemory::~InstructionMemory() {

    free(memory);
}

int InstructionMemory::loadData(int address) {

    return memory[address];
}

void InstructionMemory::loadFromFile(char *fileName) {

    std::ifstream file;
    file.open(fileName);
}