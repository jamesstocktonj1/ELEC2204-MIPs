#include "../include/decoder.h"



Decoder::Decoder() {

    curInstruction = 0;
}

Decoder::~Decoder() {
    
}

void Decoder::setCurrentInstruction(int inst) {

    curInstruction = inst;
}


int Decoder::getAddress1(void) {

    return 0x00;
}

int Decoder::getAddress2(void) {

    return 0x00;
}

int Decoder::getWriteAddress(void) {

    return 0x00;
}


ControlLines Decoder::getControlLines(void) {

    ControlLines temp;

    return temp;
}

int Decoder::getBranchAddress(void) {

    return 0x00;
}

