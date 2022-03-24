#include "../include/decoder.h"



Decoder::Decoder() {

    curInstruction = 0;
}



void Decoder::setCurrentInstruction(int inst) {

    curInstruction = inst;
}