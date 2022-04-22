#include "../include/assembler.h"



void loadFromFile(const char* file, InstructionMemory *mem) {
    FILE *f;

    char c;
    char buf[BUFFER_SIZE];
    char temp[4][8];

    //temp = (char *)malloc(sizeof(char*) * 4 * 8);

    int i = 0;
    int j = 0;


    f = fopen(file, "r");


    c = fgetc(f);
    while(c != EOF) {

        if(c == '\n') {
            buf[i] = '\0';

            Instruction inst = getInstruction(buf);
            int curInstruction = 0;
            std::cout << "Opcode: 0x" << std::hex << getOpcode(inst.opcode) << std::endl;
            std::cout << "ALU Op 0x" << std::hex << getALU(inst.opcode) << std::endl;

            //begin construct instruction
            curInstruction = (0x3f & getOpcode(inst.opcode)) << 26;

            switch (inst.instructionType) {

            case IType:
                std::cout << inst.opcode << "\trt: " << inst.rt << "\trs: " << inst.rs << "\tConst: " << inst.constant << std::endl;
                
                //rs - [25:21]
                curInstruction += (0x1f & inst.rs) << 21;

                //rt - [20:16]
                curInstruction += (0x1f & inst.rt) << 16;

                //constant - [15:0]
                curInstruction += (0xff & inst.constant);
                break;
            case JType:
                std::cout << inst.opcode << "\tJump: " << inst.constant << std::endl;
                break;
            case RType:
                std::cout << inst.opcode << "\trd: " << inst.rd << "\trs: " << inst.rs << "\trt: " << inst.rt << std::endl;
                
                //rs - [25:21]
                curInstruction += (0x1f & inst.rs) << 21;

                //rt - [20:16]
                curInstruction += (0x1f & inst.rt) << 16;

                //rd - [15:11]
                curInstruction += (0x1f & inst.rd) << 11;

                //shamt - [10:6] = 0

                //function - [5:0]
                curInstruction += (0x1f & getALU(inst.opcode));
                break;
            }

            std::cout << "Hex Instruction: 0x" << std::hex << curInstruction << std::endl;

            mem->writeMemory(j, curInstruction);

            //mem->writeMemory(j, parseInstruction(buf));
            i = 0;
            std::cout << std::endl;
        }
        else if(c == '\r');
        else {
            buf[i] = c;
            i++;
        }

        //std::cout << c;
        c = fgetc(f);
    }

    std::cout << "End of File" << std::endl;
    fclose(f);

    //mem->writeMemory(0, 10);
}


int parseInstruction(char* inst) {

    char temp[BUFFER_SIZE] = "";
    int i;

    for(i=0; i<strlen(inst); i++) {
        std::cout << inst[i];
    }

    std::cout << std::endl;
    return 10;
}

Instruction getInstruction(char* inst) {
    Instruction ins;

    std::cout << "Decode Instruction: " << inst << std::endl;

    char temp[BUFFER_SIZE];
    int i;

    int j = 0;
    int point = 0;

    for(i=0; i<strlen(inst); i++) {

        //force read last character
        if(i == (strlen(inst) - 1)) {
            temp[j] = inst[i];
            j++;
            inst[i] = ' ';
        }

        if(inst[i] == ' ' || i == (strlen(inst) - 1)) {
            temp[j] = '\0';
            j = 0;

            //write opcode
            if(point == 0) {
                strcpy(ins.opcode, temp);
                ins.instructionType = getType(ins.opcode);
            }

            //write rd or constant
            else if(point == 1) {
                int tempNum = charToInt(temp);

                if(ins.instructionType == JType) {
                    ins.constant = tempNum;
                    return ins;
                }
                else if(ins.instructionType == IType) {
                    ins.rt = tempNum;
                }
                else {
                    ins.rd = tempNum;
                }
            }

            //write rt
            else if(point == 2) {
                int tempNum = charToInt(temp);

                ins.rs = tempNum;
            }
            
            //write rd / immediate
            else if(point == 3) {
                int tempNum = charToInt(temp);

                if(ins.instructionType == IType) {
                    ins.constant = tempNum;
                }
                else {
                    ins.rt = tempNum;
                }
                return ins;
            }
            point++;
        }
        else if(inst[i] == ',');
        else if(inst[i] == '\r');
        else if(inst[i] == '\n');
        else if(inst[i] == '(');
        else if(inst[i] == ')');
        else {
            temp[j] = inst[i];
            j++;
        }
    }

    return ins;
}

int charToInt(char* num) {

    //handle hex number
    if(num[0] == '0' && num[1] == 'x') {
        return strtol(num, NULL, 16);
    }

    //handle decimal number
    else {
        return strtol(num, NULL, 10);
    }

    return 0xff;
}

InstructionType getType(char* opcode) {
    //std::cout << "Opcode: " << opcode << "\t";
    
    if(strcmp(opcode, "add") == 0) {
        //std::cout << "R-Type" << std::endl;
        return RType;
    }
    else if(strcmp(opcode, "addi") == 0) {
        //std::cout << "I-Type" << std::endl;
        return IType;
    }
    else if(strcmp(opcode, "slt") == 0) {
        //std::cout << "R-Type" << std::endl;
        return RType;
    }
    else if(strcmp(opcode, "bne") == 0) {
        //std::cout << "I-Type" << std::endl;
        return IType;
    }
    else if(strcmp(opcode, "sw") == 0) {
        //std::cout << "I-Type" << std::endl;
        return IType;
    }
    else {
        std::cout << "Error Unrecognised Instruction" << std::endl;
        return RType;
    }
}

int getOpcode(char* opcode) {

    if(strcmp(opcode, "add") == 0) {
        return 0x0;
    }
    else if(strcmp(opcode, "addi") == 0) {
        return 0x8;
    }
    else if(strcmp(opcode, "slt") == 0) {
        return 0x0;
    }
    else if(strcmp(opcode, "bne") == 0) {
        return 0x5;
    }
    else if(strcmp(opcode, "sw") == 0) {
        return 0x2b;
    }
    else {
        std::cout << "Error Unrecognised Opcode" << std::endl;
        return 0;
    }
}

int getALU(char* opcode) {

    if(strcmp(opcode, "add") == 0) {
        return ADD_OP;
    }
    else if(strcmp(opcode, "addi") == 0) {
        return ADD_OP;
    }
    else if(strcmp(opcode, "slt") == 0) {
        return SLT_OP;
    }
    else {
        std::cout << "Error Unrecognised Opcode" << std::endl;
        return 0;
    }
}


/*
void splitLine(char* inst, char* data[]) {

    //char data[6][8];
    char temp[8];
    int i, j, k;

    //temp array pointer
    j = 0;

    //lines array pointer
    k = 0;

    std::cout << "String Length: " << strlen(inst) << std::endl;

    for(i=0; i<strlen(inst); i++) {

        
        if(inst[i] == ' ') {
            //temp[j] = '\0';
            strcpy(data[k], temp);
            std::cout << temp << std::endl;
            k++;
            j = 0;
        }
        else if(inst[i] == ',');
        else {
            temp[j] = inst[i];
            //std::cout << inst[i] << std::endl;
            j++;
        }
    }
    strcpy(data[k], temp);

    //std::cout << data << std::endl;
}
*/