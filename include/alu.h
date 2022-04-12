#ifndef ALU_H
#define ALU_H



//operation constants
#define AND_OP 0b0010
#define OR_OP 0b0001
#define ADD_OP 0b0000
#define SUB_OP 0b0110
#define SLT_OP 0b1010
#define NOR_OP 0b1100


/**
 * @brief 
 * 
 */
class ALU {

public:
    ALU();

    int performComputation(int value1, int value2, int operation);
    int equalsZero(void);
    int valuesEqual(int value1, int value2);

    ~ALU();

private:
    int aluResult;
};


#endif