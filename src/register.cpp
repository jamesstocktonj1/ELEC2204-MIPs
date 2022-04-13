#include "../include/register.h"

#include <stdlib.h>
#include <string.h>


Registers::Registers() {

    memory = (int *)malloc(REGISTER_SIZE * sizeof(int));
    memset(memory, 0, REGISTER_SIZE);
}

Registers::~Registers() {

    free((int *)memory);
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