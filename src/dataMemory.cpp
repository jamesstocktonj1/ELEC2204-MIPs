#include "../includedataMemory.h"

#include <stdlib.h>


Memory::Memory() {

    memory = (int *)malloc(MEMORY_SIZE);
}

Memory::~Memory() {

    free(memory);
}

int Memory::readMemory(int addr) {

    return memory[addr];
}

void Memory::writeMemory(int addr, int data) {

    memory[addr] = data;
}