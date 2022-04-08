#ifndef DECODER_H
#define DECODER_H



typedef struct t_ControlLines {
    int regDst;
    int branch;
    int memRead;
    int memToReg;
    int aluOperation;
    int memWrite;
    int aluSrc;
    int regWrite;
} ControlLines;


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
};






#endif