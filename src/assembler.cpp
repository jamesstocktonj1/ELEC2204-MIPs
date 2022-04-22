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

            switch (inst.instructionType)
            {
            case IType:
                std::cout << inst.opcode << "\trs: " << inst.rs << "\trt: " << inst.rt << "\tConst: " << inst.constant << std::endl;
                break;
            case JType:
                std::cout << inst.opcode << "\tJump: " << inst.constant << std::endl;
                break;
            case RType:
                std::cout << inst.opcode << "\trs: " << inst.rs << "\trt: " << inst.rt << "\trd: " << inst.rd << std::endl;
            }

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

    mem->writeMemory(0, 10);
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

        if(inst[i] == ' ') {
            temp[j] = '\0';
            j = 0;

            //write opcode
            if(point == 0) {
                strcpy(ins.opcode, temp);
                ins.instructionType = getType(ins.opcode);
            }

            //write rs
            else if(point == 1) {
                int tempNum = charToInt(temp);

                if(ins.instructionType == JType) {
                    ins.constant = tempNum;
                    return ins;

                }
                else {
                    ins.rs = tempNum;
                }
            }

            //write rt
            else if(point == 2) {
                int tempNum = charToInt(temp);

                ins.rt = tempNum;
            }
            
            //write rd / immediate
            else if(point == 3) {
                int tempNum = charToInt(temp);

                if(ins.instructionType == IType) {
                    ins.constant = tempNum;
                    return ins;
                }
                else {
                    ins.rd = tempNum;
                    return ins;
                }
            }
            point++;
        }
        else if(inst[i] == ',');
        else if(inst[i] == '\r');
        else if(inst[i] == '\n');
        else {
            temp[j] = inst[i];
            j++;
        }
    }

    return ins;
}

int charToInt(char* num) {

    return 10;
}

InstructionType getType(char* opcode) {
    std::cout << "Opcode: " << opcode << "\t";
    
    if(strcmp(opcode, "add") == 0) {
        std::cout << "R-Type" << std::endl;
        return RType;
    }
    else if(strcmp(opcode, "addi") == 0) {
        std::cout << "I-Type" << std::endl;
        return IType;
    }
    else if(strcmp(opcode, "slt") == 0) {
        std::cout << "R-Type" << std::endl;
        return RType;
    }
    else if(strcmp(opcode, "bne") == 0) {
        std::cout << "I-Type" << std::endl;
        return IType;
    }
    else if(strcmp(opcode, "sw") == 0) {
        std::cout << "I-Type" << std::endl;
        return IType;
    }
    else {
        //std::cout << "Error" << std::endl;
        return RType;
    }
}



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
