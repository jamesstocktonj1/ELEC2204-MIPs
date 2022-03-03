#ifndef PC_H
#define PC_H

#include <iostream>

class PC {

public:
    PC();

    void setPC(int value);
    int getPC(void);
    void incrementPC(void);

    ~PC();

private:
    int programCounter;
};






#endif