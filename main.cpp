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

    int i;
    for(i=0; i<MEMORY_SIZE; i++) {
        registers.writeRegister(i, 0);
    }

    //final program
    instructionMemory.writeMemory(0, 0x00004020);
    instructionMemory.writeMemory(1, 0x20040010);       //loop length
    instructionMemory.writeMemory(2, 0x00004820);
    instructionMemory.writeMemory(3, 0x00005020);       //clear square value
    instructionMemory.writeMemory(4, 0x01485020);       //add t0 
    instructionMemory.writeMemory(5, 0x21290001);
    instructionMemory.writeMemory(6, 0x0128082a);
    instructionMemory.writeMemory(7, 0x14200004);       //jump 1
    instructionMemory.writeMemory(8, 0xafaa0000);
    instructionMemory.writeMemory(9, 0x21080001);
    instructionMemory.writeMemory(10, 0x23bd0001);
    instructionMemory.writeMemory(11, 0x0104082a);
    instructionMemory.writeMemory(12, 0x14200002);       //jump 2

    registers.writeRegister(10, 0xf);

    //instructionMemory.writeMemory(0, 0x00004020);
    //instructionMemory.writeMemory(1, 0x21080001);
    //instructionMemory.writeMemory(2, 0x21080001);

    //registers.writeRegister(8, 5);
    //instructionMemory.writeMemory(0, 0x21080001);
    //instructionMemory.writeMemory(1, 0x21080001);
    //instructionMemory.writeMemory(0, 0x21080001);
    //instructionMemory.writeMemory(3, 0x21080001);
    //instructionMemory.writeMemory(4, 0x21080001);

    //store value to memory
    //instructionMemory.writeMemory(5, 0xac08000a);
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
        /*
        if(mControl.branch && (wZero == mControl.branchValue)) {
            cout << "Valid Branch" << endl;
            takeBranch = 1;
        }*/
        
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
        //wPC = ePC + eJumpAddr;
        wPC = eJumpAddr;

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
        wZero = alu.valuesEqual(aluData1, aluData2);

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
        cout << "Stall: 0x" << hex << eControl.branch << endl;

        //set global branch flag
        if(eControl.branch) {
            globalBranch = 1;
            cout << "Start Stall" << endl;
        }

        //pc state

        //take branch
        if(wControl.branch && (wControl.branchValue == wZero)) {
            cout << "Branch: 0x" << hex << wPC << endl;
            cout << "0x" << hex << aluData1 << ", 0x" << hex << aluData2 << endl;

            pc.setPC(wPC);
            globalBranch = 0;
        }
        //dont take branch
        else if(wControl.branch) {
            globalBranch = 0;

            cout << "Branch Not Taken" << endl;
            cout << "0x" << hex << aluData1 << ", 0x" << hex << aluData2 << endl;
            pc.incrementPC();
        }

        //no branch but stalled
        else if(globalBranch) {
            cout << "Processor Stall" << endl;
        }
        //normal no branch
        else {
            pc.incrementPC();
            cout << "Increment PC: 0x" << hex << pc.getPC() << endl;
        }

/*
        if(globalBranch) {

            if(takeBranch) {
                pc.setPC(wPC);
                
                globalBranch = 0;
                takeBranch = 0;
            }
            else if(wControl.branch) {
                cout << "Branch Not Taken" << endl;
                pc.incrementPC();
                cout << "Increment PC: 0x" << hex << pc.getPC() << endl;
                globalBranch = 0;
                takeBranch = 0;
            }
        }

        //normal non-branch mode
        else {
            pc.incrementPC();
            cout << "Increment PC: 0x" << hex << pc.getPC() << endl;

            globalBranch = 0;
            takeBranch = 0;
        }*/

        dPC = pc.getPC();
        dInstruction = (globalBranch) ? 0 : instructionMemory.loadData(dPC);

        //program finishes at 
        if((dPC == 0x10) && (globalBranch == 0)) {

            cout << "Dumping Memory" << endl;
            memory.dumpFile();
            cout << "Program Exit" << endl;
            return 0;
        }

        //cout << "\nRegister Value: 0x" << hex << registers.readRegister(8) << "\n\n";
        cout << "$a0: 0x" << hex << registers.readRegister(4) << "\n";
        cout << "$t0: 0x" << hex << registers.readRegister(8) << "\n";
        cout << "$t1: 0x" << hex << registers.readRegister(9) << "\n";
        cout << "$t2: 0x" << hex << registers.readRegister(10) << "\n";
        cout << "$sp: 0x" << hex << registers.readRegister(29) << "\n";

        cout << "Instruction Finished!\n" << endl;
        sleep(1);
        
    }
}



