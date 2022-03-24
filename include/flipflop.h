#ifndef FLIPFLOP_H
#define FLIPFLOP_H

#include "decoder.h"



class FlipFlop {

public:
    FlipFlop();

    int curValue(void);
    void takeValue(int value);

    ~FlipFlop();

private:
    int storedValue;
};



class ControlFlipFlop {

public:
    ControlFlipFlop();

    ControlLines curValue(void);
    void takeValue(ControlLines value);

    ~ControlFlipFlop();

private:
    ControlLines storedValue;
};

#endif