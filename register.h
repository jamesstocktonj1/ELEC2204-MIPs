#ifndef REGISTER_H
#define REGISTER_H

#define REGISTER_SIZE 32


class Registers {

public:
    Registers();

    int readRegister(int addr);
    void writeRegister(int addr, int data);

    ~Registers();

private:
    int *memory;
};


#endif