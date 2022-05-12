#include <iostream>
#include <unistd.h>

#include "include/assembler.h"

#include "include/pc.h"
#include "include/instructionMemory.h"
#include "include/decoder.h"
#include "include/register.h"
#include "include/alu.h"
#include "include/dataMemory.h"



using namespace std;


//program defines
//#define STEP_THROUGH
//#define VERBOSE
#define ASSEMBLE

void manualProgramLoad(void);

void addiTest(void);
void branchTest(void);

void printHelp(void);
int parseArguments(char **argv, int argc);


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

//collision variables
int writtenRegisterAddress;
int writtenRegister;
int writterRegisterFlag;

int globalBranch;

int instructionCount;


char assemblyFile[32] = "asm/MIPsBasicSquares.asm";



int main(int argc, char **argv) {

    if(parseArguments(argv, argc)) {
        return 0;
    }

    return 0;

	//either load from assembler or manual write to instruction memory
	#ifdef ASSEMBLE
	loadFromFile(assemblyFile, &instructionMemory);
	#else
    manualProgramLoad();
    //addiTest();
    //branchTest();
	#endif
    
    //setup values
    pc.setPC(0);
    globalBranch = 0;

    //load first instruction
    dInstruction = instructionMemory.loadData(0);

    instructionCount = 0;

    while(1) {
        instructionCount++;

        //memory write back
        if(mControl.regWrite) {

            //handle register writeback
            registers.writeRegister(mDestAddr, ((mControl.memToReg) ? mResult : mData));

            //log if not writing to $zero
			#ifdef VERBOSE
            if(mDestAddr != 0) {
                cout << "Write Register 0x" << hex << mDestAddr << ": 0x" << hex << ((mControl.memToReg) ? mResult : mData) << endl;
            }
			#endif
        }
        
        //collision control stored values
        writtenRegisterAddress = mDestAddr;
        writtenRegister = ((mControl.memToReg) ? mResult : mData);
        writterRegisterFlag = mControl.regWrite;


        //data memory
        mControl = wControl;
        
        //handle memory write
        if(wControl.memWrite) {
            memory.writeMemory(wResult, wData2);

			#ifdef VERBOSE
            cout << "Write Memory 0x" << hex << wResult << ": 0x" << hex << wData2 << endl;
			#endif
        }
        //handle memory read
        if(wControl.memRead) {
            mData = memory.readMemory(wData2);

			#ifdef VERBOSE
            cout << "Read Memory 0x" << hex << wDestAddr << ": 0x" << hex << mData << endl;
			#endif
        }
        mResult = wResult;
        mDestAddr = wDestAddr;


        //execute
        wControl = eControl;
        wPC = eJumpAddr;

        //get alu data (collision control)
        aluData1 = eData1;
        aluData2 = eData2;

        //data 1 collision control
        if(mControl.regWrite && (mDestAddr == eDataAddress1) && (eDataAddress1 != 0)) {
            aluData1 = wResult;

			#ifdef VERBOSE
            cout << "Data 1: wResult 0x" << hex << aluData1 << endl;
			#endif
        }
        else if(writterRegisterFlag && (writtenRegisterAddress == eDataAddress1) && (eDataAddress1 != 0)) {
            aluData1 = writtenRegister;

			#ifdef VERBOSE
            cout << "Data 1: writtenRegister 0x" << hex << aluData1 << endl;
			#endif
        }

        //data 2 colission control
        if(mControl.regWrite && (mDestAddr == eDataAddress2) && (eDataAddress2 != 0)) {
            aluData2 = wResult;

			#ifdef VERBOSE
            cout << "Data 2: wResult 0x" << hex << aluData2 << endl;
			#endif
        }
        else if(writterRegisterFlag && (writtenRegisterAddress == eDataAddress2) && (eDataAddress2 != 0)) {
            aluData2 = writtenRegister;

			#ifdef VERBOSE
            cout << "Data 2: writtenRegister 0x" << hex << aluData2 << endl;
			#endif
        }

        wData2 = aluData2;
        aluData2 = (eControl.aluSrc) ? eJumpAddr : aluData2;

        wResult = alu.performComputation(aluData1, aluData2, eControl.aluOperation);
        wZero = alu.valuesEqual(aluData1, aluData2);

		#ifdef VERBOSE
        //ALU Logging
        cout << "ALU: 0x" << hex << aluData1;
        cout << ", 0x" << hex <<  aluData2;
        cout << ", 0x" << hex << eControl.aluOperation;
        cout << ": 0x" << hex << wResult << endl;
		#endif

        wDestAddr = eDestAddr;


        //decoder state
        decoder.setCurrentInstruction(dInstruction);
        eControl = decoder.getControlLines();
        ePC = dPC;

        eDataAddress1 = decoder.getAddress1();
        eDataAddress2 = decoder.getAddress2();
        eData1 = registers.readRegister(eDataAddress1);
        eData2 = registers.readRegister(eDataAddress2);

        eJumpAddr = decoder.getBranchAddress();// << 16;
        eDestAddr = decoder.getWriteAddress();

		#ifdef VERBOSE
        cout << "Decoder: 0x" << hex << eDataAddress1;
        cout << ", 0x" << hex << eDataAddress2;
        cout << ", 0x" << hex << eJumpAddr;
        cout << ", 0x" << hex << eDestAddr << endl;
		#endif

        //set global branch flag
        if(eControl.branch) {
            globalBranch = 1;

			#ifdef VERBOSE
            cout << "Start Stall" << endl;
			#endif
        }


        //pc state

        //take branch
        if(wControl.branch && (wControl.branchValue == wZero)) {

			#ifdef VERBOSE
            cout << "Branch: 0x" << hex << wPC << endl;
            cout << "0x" << hex << aluData1 << ", 0x" << hex << aluData2 << endl;
			#endif
            pc.setPC(wPC);
            globalBranch = 0;
        }
        //dont take branch
        else if(wControl.branch) {
            globalBranch = 0;

			#ifdef VERBOSE
            cout << "Branch Not Taken" << endl;
			#endif
            pc.incrementPC();
        }

        //no branch but stalled
        else if(globalBranch) {

			#ifdef VERBOSE
            cout << "Processor Stall" << endl;
			#endif
        }
        //normal no branch
        else {
            pc.incrementPC();

			#ifdef VERBOSE
            cout << "Increment PC: 0x" << hex << pc.getPC() << endl;
			#endif
        }

        //retrieve current pc and instruction
        dPC = pc.getPC();
        dInstruction = (globalBranch) ? 0 : instructionMemory.loadData(dPC);

        //handle program finish ()
        if((dPC == 0x10) && (globalBranch == 0)) {

            cout << "Total Instruction Count: " << dec << instructionCount << endl;

            cout << "Dumping Memory" << endl;
            memory.dumpFile();
            cout << "Program Exit" << endl;
            return 0;
        }

		#ifdef VERBOSE
        cout << "$a0: 0x" << hex << registers.readRegister(4) << "\n";
        cout << "$t0: 0x" << hex << registers.readRegister(8) << "\n";
        cout << "$t1: 0x" << hex << registers.readRegister(9) << "\n";
        cout << "$t2: 0x" << hex << registers.readRegister(10) << "\n";
        cout << "$sp: 0x" << hex << registers.readRegister(29) << "\n";

        cout << "Instruction Finished!\n" << endl;
		#endif
        

		#ifdef STEP_THROUGH
        sleep(1);
		#endif
    }
}



void manualProgramLoad() {

	//final program
    instructionMemory.writeMemory(0, 0x00004020);
    instructionMemory.writeMemory(1, 0x200400c8);       //loop length
    instructionMemory.writeMemory(2, 0x00004820);
    instructionMemory.writeMemory(3, 0x00005020);       //clear square value
    instructionMemory.writeMemory(4, 0x010a5020);       //add t2
    instructionMemory.writeMemory(5, 0x21290001);
    instructionMemory.writeMemory(6, 0x0128082a);
    instructionMemory.writeMemory(7, 0x14200004);       //jump 1
    //instructionMemory.writeMemory(8, 0xafaa0000);       //store word ($sp)
    instructionMemory.writeMemory(8, 0xad0a0000);       //store word ($t0)
    instructionMemory.writeMemory(9, 0x21080001);
    //instructionMemory.writeMemory(10, 0x23bd0001);        //increment $sp
    instructionMemory.writeMemory(10, 0x0104082a);
    instructionMemory.writeMemory(11, 0x14200002);       //jump 2
}


void addiTest() {

    instructionMemory.writeMemory(0, 0x20080005);
    instructionMemory.writeMemory(1, 0x21290001);
    instructionMemory.writeMemory(2, 0x15090001);
}

void branchTest() {

    instructionMemory.writeMemory(0, 0x21290001);
    instructionMemory.writeMemory(1, 0x21290001);

    instructionMemory.writeMemory(3, 0x21290001);
}




void printHelp() {

    cout << "MIPs Processor" << endl;
    cout << "\t-a <file>\t- changes the assembly program which is loaded into memory" << endl;
    cout << "\t-c\t\t- puts the processor into console mode allowing you to input commands" << endl;
}


int parseArguments(char **argv, int argc) {


    for(int i=1; i<argc; i++) {

        if(argv[i][0] == '-') {
            switch(argv[i][1]) {
                case 'a':
                    strcpy(assemblyFile, argv[i + 1]);
                    i++;
                    cout << "Using Assembly File: " << assemblyFile << endl;
                    break;
                case 'c':
                    cout << "Argument Console" << endl;
                    break;
                case 'h':
                    printHelp();
                    return 1;
                    break;

            }
        }

        
        cout << "Argument " << i << ": " << argv[i] << endl;
    }

    return 0;
}