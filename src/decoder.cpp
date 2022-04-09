#include "../include/decoder.h"
#include <iostream>



Decoder::Decoder() {

    curInstruction = 0;

    address1 = 0;
    address2 = 0;
    writeAddress = 0;

    branchAddress = 0;
}

Decoder::~Decoder() {
    
}

void Decoder::setCurrentInstruction(int inst) {

    curInstruction = inst;

    //initialise variables
    controlLines = {0, };
    address1 = 0;
    address2 = 0;
    writeAddress = 0;
    branchAddress = 0;

    //op code - [31:26] (6-bits)
    opCode = 0x3f & (curInstruction >> 26);

    std::cout << "Instruction: 0x" << std::hex << curInstruction << std::endl;
    std::cout << "Op Code: 0x" << std::hex << opCode << std::endl;

    //R - type instruction
    if(1) {
    //if(opCode == 0) {

        //funct - [5:0] (6-bits)
        controlLines.aluOperation = curInstruction & 0x3f;

        //shamt - [10:6] (5-bits)

        //rd - [15:11] (5-bits)
        controlLines.regWrite = 0x1f & (curInstruction >> 11);

        //rt - [20:16] (5-bits)
        address1 = 0x1f & (curInstruction >> 16);

        //rs - [25:21] (5-bits)
        address2 = 0x1f & (curInstruction >> 21);

        //set flow control
        controlLines.regDst = 1;
        controlLines.regWrite = 1;
    }
}


int Decoder::getAddress1(void) {

    return address1;
}

int Decoder::getAddress2(void) {

    return address2;
}

int Decoder::getWriteAddress(void) {

    return writeAddress;
}


ControlLines Decoder::getControlLines(void) {

    return controlLines;
}

int Decoder::getBranchAddress(void) {

    return branchAddress;
}

