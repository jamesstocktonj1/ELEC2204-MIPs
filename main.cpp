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
int eDataAddress1;
int eDataAddress2;
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

//localised data
int aluData1;
int aluData2;

int writtenRegisterAddress;
int writtenRegister;
int writterRegisterFlag;

int globalBranch;





int instMem[12] = {
    0,
};



int main() {

    pc.setPC(0);

    //instructionMemory.writeMemory(0, 0x00004020);
    //instructionMemory.writeMemory(1, 0x21080001);
    //instructionMemory.writeMemory(2, 0x21080001);

    //registers.writeRegister(8, 5);
    instructionMemory.writeMemory(0, 0x21080001);
    instructionMemory.writeMemory(1, 0x21080001);
    instructionMemory.writeMemory(0, 0x21080001);
    instructionMemory.writeMemory(3, 0x21080001);
    instructionMemory.writeMemory(4, 0x21080001);

    //store value to memory
    instructionMemory.writeMemory(5, 0xac08000a);
    //instructionMemory.writeMemory(6, 0xff);

    

    //instructionMemory.writeMemory(5, 0x21080001);

    //instructionMemory.writeMemory(5, 0x0100402a);

    globalBranch = 0;




    //load first instruction
    dInstruction = instructionMemory.loadData(0);

    while(1) {

        //memory write back
        if(mControl.regWrite) {

            registers.writeRegister(mDestAddr, ((mControl.memToReg) ? mResult : mData));

            if(mDestAddr != 0) {
                cout << "Write Register 0x" << hex << mDestAddr << ": 0x" << hex << ((mControl.memToReg) ? mResult : mData) << endl;
            }
        }
        
        //colission control stored values
        writtenRegisterAddress = mDestAddr;
        writtenRegister = ((mControl.memToReg) ? mResult : mData);
        writterRegisterFlag = mControl.regWrite;


        //data memory
        mControl = wControl;
        
        if(wControl.memWrite) {
            memory.writeMemory(wResult, wData2);
            cout << "Write Memory 0x" << hex << wResult << ": 0x" << hex << wData2 << endl;
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

        //get alu data (collision control)
        aluData1 = eData1;
        aluData2 = eData2;
        

        //data 1 collision control
        if(mControl.regWrite && (mDestAddr == eDataAddress1) && (eDataAddress1 != 0)) {
            aluData1 = wResult;
            cout << "Data 1: wResult 0x" << hex << aluData1 << endl; 
        }
        else if(writterRegisterFlag && (writtenRegisterAddress == eDataAddress1) && (eDataAddress1 != 0)) {
            aluData1 = writtenRegister;
            cout << "Data 1: writtenRegister 0x" << hex << aluData1 << endl; 
        }

        //data 2 colission control
        if(mControl.regWrite && (mDestAddr == eDataAddress2) && (eDataAddress2 != 0)) {
            aluData2 = wResult;
            cout << "Data 2: wResult 0x" << hex << aluData2 << endl; 
        }
        else if(writterRegisterFlag && (writtenRegisterAddress == eDataAddress2) && (eDataAddress2 != 0)) {
            aluData2 = writtenRegister;
            cout << "Data 2: writtenRegister 0x" << hex << aluData2 << endl;
        }

        wData2 = aluData2;
        aluData2 = (eControl.aluSrc) ? eJumpAddr : aluData2;

        wResult = alu.performComputation(aluData1, aluData2, eControl.aluOperation);
        wZero = alu.equalsZero();

        //ALU Logging
        cout << "ALU: 0x" << hex << aluData1;
        cout << ", 0x" << hex <<  aluData2;
        cout << ", 0x" << hex << eControl.aluOperation;
        cout << ": 0x" << hex << wResult << endl;

        
        wDestAddr = eDestAddr;

        //decoder state
        decoder.setCurrentInstruction(dInstruction);
        //decoder.setCurrentInstruction(0x00004020);
        eControl = decoder.getControlLines();
        ePC = dPC;

        eDataAddress1 = decoder.getAddress1();
        eDataAddress2 = decoder.getAddress2();
        eData1 = registers.readRegister(eDataAddress1);
        eData2 = registers.readRegister(eDataAddress2);

        eJumpAddr = decoder.getBranchAddress();// << 16;
        eDestAddr = decoder.getWriteAddress();

        cout << "Decoder: 0x" << hex << eDataAddress1;
        cout << ", 0x" << hex << eDataAddress2;
        cout << ", 0x" << hex << eJumpAddr;
        cout << ", 0x" << hex << eDestAddr << endl;

        //set global branch flag
        if(eControl.branch) {
            globalBranch = 1;
        }

        //pc state
        if(wControl.branch) {
            pc.setPC(wPC);
            cout << "Branch: 0x" << hex << wPC << endl;
        }

        //normal non-branch mode
        else {
            pc.incrementPC();
            cout << "Increment PC: 0x" << hex << pc.getPC() << endl;

            globalBranch = 0;
        }

        dPC = pc.getPC();
        dInstruction = (globalBranch) ? 0 : instructionMemory.loadData(dPC);

        //program finishes at 
        if((dPC == 0xa) && (globalBranch == 0)) {

            cout << "Dumping Memory" << endl;
            memory.dumpFile();
            cout << "Program Exit" << endl;
            return 0;
        }

        cout << "\nRegister Value: 0x" << hex << registers.readRegister(8) << "\n\n";

        cout << "Instruction Finished!\n" << endl;
        sleep(1);
        
    }
}



