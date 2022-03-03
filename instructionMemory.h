#ifndef INSTRUCTION_H
#define INSTRUCTION_H



class InstructionMemory {

public:
    InstructionMemory();

    int loadData(int address);
    void loadFromFile(char *fileName);

    ~InstructionMemory();
};



#endif