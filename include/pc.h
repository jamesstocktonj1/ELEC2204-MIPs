#ifndef PC_H
#define PC_H

#include <iostream>

#define INCREMENT_SIZE 1

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