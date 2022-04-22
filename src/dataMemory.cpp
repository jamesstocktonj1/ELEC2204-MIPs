#include "../include/dataMemory.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>


Memory::Memory() {

    memory = (int *)malloc(MEMORY_SIZE * sizeof(int));
    memset(memory, 0, MEMORY_SIZE);
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

        #ifdef DUMP_HEX
        sprintf(temp, "0x%04x", memory[i]);
        memoryFile << temp << " ";

        #else
        sprintf(temp, "% 6d", memory[i]);
        memoryFile << temp;
        #endif
        

        if(((i + 1) % 16) == 0) {
            memoryFile << "\n";
        }
    }

    memoryFile.close();
}