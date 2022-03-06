#ifndef DATAMEMORY_H
#define DATAMEMORY_H

#define MEMORY_SIZE 256


class Memory {

public:
    Memory();

    int readMemory(int addr);
    void writeMemory(int addr, int data);

    ~Memory();

private:
    int *memory;
};


#endif