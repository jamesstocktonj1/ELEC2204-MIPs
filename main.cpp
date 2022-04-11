#include <iostream>
#include <unistd.h>

//fetch
#include "include/pc.h"
#include "include/instructionMemory.h"

//decode
#include "include/decoder.h"
#include "include/register.h"

//execute
#include "include/alu.h"

//writeback
#include "include/dataMemory.h"



using namespace std;




//define main MIPS Processor blocks
PC pc;
InstructionMemory instructionMemory;

Decoder decoder;
Registers registers;

ALU alu;

Memory memory;




//main state storage variables
//decode state
int dPC;
int dInstruction;

//execute
ControlLines eControl;
int ePC;
int eData1;
int eData2;
int eJumpAddr;
int eDestAddr;

//writeback
ControlLines wControl;
int wPC;
int wZero;
int wResult;
int wData2;
int wDestAddr;

//memory write back
ControlLines mControl;
int mData;
int mResult;
int mDestAddr;


int instMem[12] = {
    0,
};



int main() {

    pc.setPC(0);

    //instructionMemory.writeMemory(0, 0x00004020);
    instructionMemory.writeMemory(0, 0x21080001);

    instructionMemory.writeMemory(5, 0x0100402a);




    //load first instruction
    dInstruction = instructionMemory.loadData(0);

    while(1) {

        //memory write back
        if(mControl.regWrite) {
            registers.writeRegister(mDestAddr, ((mControl.memToReg) ? mResult : mData));
            cout << "Write Register 0x" << hex << mDestAddr << ": 0x" << hex << ((mControl.memToReg) ? mResult : mData) << endl;
        }


        //data memory
        mControl = wControl;
        
        if(wControl.memWrite) {
            memory.writeMemory(wData2, wResult);
            cout << "Write Memory 0x" << hex << wDestAddr << ": 0x" << hex << wData2 << endl;
        }
        if(wControl.memRead) {
            mData = memory.readMemory(wData2);
            cout << "Read Memory 0x" << hex << wDestAddr << ": 0x" << hex << mData << endl;
        }
        mResult = wResult;
        mDestAddr = wDestAddr;


        //execute
        wControl = eControl;
        wPC = ePC + eJumpAddr;

        wResult = alu.performComputation(eData1, ((eControl.aluSrc) ? eJumpAddr : eData2), eControl.aluOperation);
        wZero = alu.equalsZero();

        //ALU Logging
        cout << "ALU: 0x" << hex << eData1;
        cout << ", 0x" << hex <<  ((eControl.aluSrc) ? eJumpAddr : eData2);
        cout << ", 0x" << hex << eControl.aluOperation;
        cout << ": 0x" << hex << wResult << endl;

        wData2 = eData2;
        wDestAddr = eDestAddr;

        //decoder state
        decoder.setCurrentInstruction(dInstruction);
        //decoder.setCurrentInstruction(0x00004020);
        eControl = decoder.getControlLines();
        ePC = dPC;

        eData1 = registers.readRegister(decoder.getAddress1());
        eData2 = registers.readRegister(decoder.getAddress2());

        eJumpAddr = decoder.getBranchAddress();// << 16;
        eDestAddr = decoder.getWriteAddress();
        

        //pc state
        if(wControl.branch) {
            pc.setPC(wPC);
            cout << "Branch: 0x" << hex << wPC << endl;
        }
        else {
            pc.incrementPC();
            cout << "Increment PC: 0x" << hex << pc.getPC() << endl;
        }
        dPC = pc.getPC();
        dInstruction = instructionMemory.loadData(dPC);

        cout << "\nRegister Value: 0x" << hex << registers.readRegister(8) << "\n\n";

        cout << "Instruction Finished!\n" << endl;
        sleep(1);

        
    }
}



