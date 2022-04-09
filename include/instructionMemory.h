#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define INSTRUCTION_MEMORY_SIZE 32

class InstructionMemory {

public:
    InstructionMemory();

    int loadData(int address);
    void loadFromFile(char *fileName);

    void writeMemory(int address, int value);

    ~InstructionMemory();

private:
    int *memory;
};



#endif