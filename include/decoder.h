#ifndef DECODER_H
#define DECODER_H



typedef struct t_ControlLines {
    int regDst = 0;
    int branch = 0;
    int memRead = 0;
    int memToReg = 0;
    int aluOperation = 0;
    int memWrite = 0;
    int aluSrc = 0;
    int regWrite = 0;
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

    int address1;
    int address2;
    int writeAddress;
    
    ControlLines controlLines;

    int branchAddress;

private:

    int curInstruction;
    int opCode;
};


#endif