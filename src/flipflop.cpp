#include "../includeflipflop.h"


FlipFlop::FlipFlop() {

    storedValue = 0;
}

FlipFlop::~FlipFlop() {

}

int FlipFlop::curValue() {

    return storedValue;
}

void FlipFlop::takeValue(int value) {

    storedValue = value;
}


ControlFlipFlop::ControlFlipFlop() {

    storedValue = ControlLines();
}

ControlFlipFlop::~ControlFlipFlop() {

}

ControlLines ControlFlipFlop::curValue() {

    return storedValue;
}

void ControlFlipFlop::takeValue(ControlLines value) {

    storedValue = value;
}