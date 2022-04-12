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

    //no instruction
    if(inst == 0);

    //R - type instruction
    else if(opCode == 0) {
    //if(1) {

        //funct - [5:0] (6-bits)
        controlLines.aluOperation = inst & 0x3f;

        //shamt - [10:6] (5-bits)

        //rd - [15:11] (5-bits)
        writeAddress = 0x1f & (inst >> 11);

        //rt - [20:16] (5-bits)
        address1 = 0x1f & (inst >> 16);

        //rs - [25:21] (5-bits)
        address2 = 0x1f & (inst >> 21);

        //set flow control
        controlLines.regDst = 1;
        controlLines.regWrite = 1;
        controlLines.memToReg = 1;
    }

    //addi instruction
    else if(opCode == 0x8) {

        //alu - add operation
        controlLines.aluOperation = 0x0;

        //constant - [15:0]
        branchAddress = 0xff & inst;

        //rd - [20:16]
        writeAddress = 0x1f & (inst >> 16);

        //rs - [25:21]
        address1 = 0x1f & (inst >> 21);


        //set flow control
        controlLines.aluSrc = 1;
        controlLines.regDst = 1;
        controlLines.regWrite = 1;
        controlLines.memToReg = 1;
    }

    //sw instruction
    else if(opCode == 0x2b) {

        //alu - add operation
        controlLines.aluOperation = 0x0;

        //constant - [15:0] unused
        branchAddress = inst & 0xff;

        //rt - [20:16] value
        address2 = 0x1f & (inst >> 16);

        //rs - [25:21] address
        address1 = 0x1f & (inst >> 21);

        //aluResult = rs + branchAddress = rs
        //branchAddress = 0x0;


        //set flow control
        controlLines.aluSrc = 1;
        controlLines.memWrite = 1;
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

