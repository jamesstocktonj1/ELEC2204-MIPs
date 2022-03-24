#ifndef DECODER_H
#define DECODER_H



typedef struct ControlLines {
    int regDst;
    int branch;
    int memRead;
    int memToReg;
    int aluOperation;
    int memWrite;
    int aluSrc;
    int regWrite;
};


class Decoder {

public:
    Decoder();

    void setCurrentInstruction(int inst);

    int getAddress1(void);
    int getAddress2(void);
    int getWriteAddress(void);

    ControlLines getControlLines(void);

    int getBranchAddress(void);

    ~Decoder();

private:

    int curInstruction;
}






#endif