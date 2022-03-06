#include <iostream>

#include "instructionMemory.h"
#include "pc.h"
#include "dataMemory.h"
#include "register.h"


using namespace std;

void testRegister(void);
void testMemory(void);
void testPC(void);


int main() {


    cout << "Performing Tests..." << endl;


    testRegister();
    testMemory();
    testPC();

    return 0;
}





void testRegister() {
    
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
}


void testMemory() {
    
    cout << "\nMemory Test" << endl;

    Registers reg;

    int writeValue = 0xf0;
    int writeAddr = 0x01;


    cout << "Writing value " << writeValue << " to memory address " << writeAddr << endl;
    reg.writeRegister(writeAddr, writeValue);

    cout << "Reading memory address " << writeAddr << endl;
    int readValue = reg.readRegister(writeAddr);


    bool passTest = true;

    if(readValue != writeValue) {
        cout << "Memory Read/Write values do not match" << endl;
        passTest = false;
    }

    //print whether task passed / failed
    cout << "Test: " << ((passTest) ? "PASS" : "FAIL") << endl;
}


void testPC() {

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

    if(incrementedValue != 4) {
        cout << "PC failed to increment by 4" << endl;
        passTest = false;
    }

    if(jumpValue != jumpAddr) {
        cout << "Failed to set PC to jump address" << endl;
        passTest = false;
    }

    //print whether task passed / failed
    cout << "Test: " << ((passTest) ? "PASS" : "FAIL") << endl;
}