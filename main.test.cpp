#include <iostream>

#include "include/instructionMemory.h"
#include "include/pc.h"
#include "include/dataMemory.h"
#include "include/register.h"
#include "include/alu.h"


using namespace std;

bool testRegister(void);
bool testMemory(void);
bool testPC(void);
bool testALU(void); 

int main() {


    cout << "Performing Tests..." << endl;

    bool fullPass = true;

    fullPass = testRegister() & fullPass;
    fullPass = testMemory() & fullPass;
    fullPass = testPC() & fullPass;
    fullPass = testALU() & fullPass;

    cout << "\nComplete System Test" << endl;
    cout << "Full Test: " << ((fullPass) ? "PASS" : "FAIL") << endl;

    return 0;
}





bool testRegister() {
    
    cout << "\nRegister Test" << endl;

    Registers reg;

    int writeValue = 0xf0;
    int writeAddr = 0x01;


    cout << "Writing value " << writeValue << " to register " << writeAddr << endl;
    reg.writeRegister(writeAddr, writeValue);

    cout << "Reading register " << writeAddr << endl;
    int readValue = reg.readRegister(writeAddr);


    cout << "Writing value to $zero register" << endl;
    reg.writeRegister(0, 10);

    cout << "Reading $zero register" << endl;
    int zeroValue = reg.readRegister(0);


    bool passTest = true;

    if(zeroValue != 0) {
        cout << "Zero register is not zero" << endl;
        passTest = false;
    }

    if(readValue != writeValue) {
        cout << "Register Read/Write values do not match" << endl;
        passTest = false;
    }

    //print whether task passed / failed
    cout << "Test: " << ((passTest) ? "PASS" : "FAIL") << endl;
    return passTest;
}


bool testMemory() {
    
    cout << "\nMemory Test" << endl;

    Memory mem;

    int writeValue = 0xf0;
    int writeAddr = 0x01;


    cout << "Writing value " << writeValue << " to memory address " << writeAddr << endl;
    mem.writeMemory(writeAddr, writeValue);

    for(int i=0; i<64; i++) {
        mem.writeMemory(i + 192, (i << 8) | i);
    }

    cout << "Reading memory address " << writeAddr << endl;
    int readValue = mem.readMemory(writeAddr);


    bool passTest = true;

    if(readValue != writeValue) {
        cout << "Memory Read/Write values do not match" << endl;
        passTest = false;
    }

    //dump memory contents to text file
    mem.dumpFile();

    //print whether task passed / failed
    cout << "Test: " << ((passTest) ? "PASS" : "FAIL") << endl;
    return passTest;
}


bool testPC() {

    cout << "\nPC Test" << endl;

    PC pc;
    int jumpAddr = 12;

    cout << "Reading start value" << endl;
    int startValue = pc.getPC();

    cout << "Incrementing value" << endl;
    pc.incrementPC();
    int incrementedValue = pc.getPC();

    cout << "Setting jump address" << endl;
    pc.setPC(jumpAddr);
    int jumpValue = pc.getPC();


    bool passTest = true;

    if(startValue != 0) {
        cout << "PC failed to initialise as 0" << endl;
        passTest = false;
    }

    if(incrementedValue != 1) {
        cout << "PC failed to increment by 1" << endl;
        passTest = false;
    }

    if(jumpValue != jumpAddr) {
        cout << "Failed to set PC to jump address" << endl;
        passTest = false;
    }

    //print whether task passed / failed
    cout << "Test: " << ((passTest) ? "PASS" : "FAIL") << endl;
    return passTest;
}



bool testALU() {

    cout << "\nALU Test" << endl;

    ALU alu;
    int valueA = 26;
    int valueB = 12;

    cout << "Testing Operations:" << endl;

    cout << "AND" << endl;
    int andResult = alu.performComputation(valueA, valueB, AND_OP);

    cout << "OR" << endl;
    int orResult = alu.performComputation(valueA, valueB, OR_OP);

    cout << "ADD" << endl;
    int addResult = alu.performComputation(valueA, valueB, ADD_OP);

    cout << "SUB" << endl;
    int subResult = alu.performComputation(valueA, valueB, SUB_OP);

    cout << "SLT" << endl;
    int sltResult = alu.performComputation(valueA, valueB, SLT_OP);

    cout << "NOR" << endl;
    int norResult = alu.performComputation(valueA, valueB, NOR_OP);

    cout << "ZERO" << endl;
    int zeroResult = alu.performComputation(valueA, valueA, SUB_OP);


    bool passTest = true;

    if(andResult != (valueA & valueB)) {
        cout << "AND failed" << endl;
        passTest = false;
    }

    if(orResult != (valueA | valueB)) {
        cout << "OR failed" << endl;
        passTest = false;
    }

    if(addResult != (valueA + valueB)) {
        cout << "ADD failed" << endl;
        passTest = false;
    }

    if(subResult != (valueA - valueB)) {
        cout << "SUB failed" << endl;
        passTest = false;
    }

    if(sltResult != ((valueA > valueB) ? 1 : 0)) {
        cout << "SLT failed" << endl;
        passTest = false;
    }

    if(norResult != ~(valueA | valueB)) {
        cout << "NOR failed" << endl;
        passTest = false;
    }

    //print whether task passed / failed
    cout << "Test: " << ((passTest) ? "PASS" : "FAIL") << endl;
    return passTest;
}