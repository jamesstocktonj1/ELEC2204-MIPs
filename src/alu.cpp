#include "../include/alu.h"
#include <iostream>


ALU::ALU() {

}

ALU::~ALU() {

}


int ALU::performComputation(int value1, int value2, int operation) {

    //execute arithmetic based on operation
    switch(operation) {
        case AND_OP:
            aluResult = value1 & value2;
            break;
        
        case OR_OP:
            aluResult = value1 | value2;
            break;

        case ADD_OP:
            aluResult = value1 + value2;
            break;
        
        case SUB_OP:
            aluResult = value1 - value2;
            break;

        case SLT_OP:
            aluResult = (value1 > value2) ? 1 : 0;
            break;

        case NOR_OP:
            aluResult =  ~(value1 | value2);
            break;

        default:
            aluResult = 0;
            break;
    }

    return aluResult;
}

int ALU::equalsZero() {

    return (aluResult == 0);
}

int ALU::valuesEqual(int value1, int value2) {

    return (value1 == value2);
}