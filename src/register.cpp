#include "../includeregister.h"

#include <stdlib.h>


Registers::Registers() {

    memory = (int *)malloc(REGISTER_SIZE);
}

Registers::~Registers() {

    free(memory);
}

int Registers::readRegister(int addr) {

    //implement $zero register
    if(addr == 0) {
        return 0;
    }

    //read from memory
    else {
        return memory[addr];
    }
}

void Registers::writeRegister(int addr, int data) {

    //write value "data" to address "addr"
    memory[addr] = data;
}