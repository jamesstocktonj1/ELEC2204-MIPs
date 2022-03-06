#include "pc.h"




PC::PC() {
    programCounter = 0;
}

void PC::setPC(int value) {
    programCounter = value;
}

int PC::getPC() {
    return programCounter;
}

void PC::incrementPC() {
    programCounter += INCREMENT_SIZE;
}

PC::~PC() {
    
}