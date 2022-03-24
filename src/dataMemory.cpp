#include "../include/dataMemory.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>


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


void Memory::dumpFile() {

    std::fstream memoryFile;
    char temp[8];

    memoryFile.open("memoryDump.txt", std::ios::out);

    for(int i=0; i<MEMORY_SIZE; i++) {
        sprintf(temp, "%X", memory[i]);
        memoryFile << temp << " ";

        if((i - 1) % 8 == 0) {
            memoryFile << "\n";
        }
    }

    memoryFile.close();
}