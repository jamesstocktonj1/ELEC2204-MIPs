#include "../include/assembler.h"



void loadFromFile(const char* file, InstructionMemory *mem) {
    FILE *f;

    //temporary char and buffer (file read)
    char c;
    char buf[BUFFER_SIZE];

    //buffer point
    int i = 0;

    //instruction memory point
    int j = 0;

    //open file as read
    f = fopen(file, "r");

    c = fgetc(f);

    //loop through file
    while(c != EOF) {

        //if end of line then process buffer
        if(c == '\n') {
            buf[i] = '\0';
            i = 0;

            //deconstruct instruction from char buffer
            Instruction inst = getInstruction(buf);
            int curInstruction = 0;

            //begin construct instruction
            curInstruction = (0x3f & getOpcode(inst.opcode)) << 26;

            //construct instruction depending on type
            switch (inst.instructionType) {
                
                case IType:
                    //rs - [25:21]
                    curInstruction += (0x1f & inst.rs) << 21;

                    //rt - [20:16]
                    curInstruction += (0x1f & inst.rt) << 16;

                    //constant - [15:0]
                    curInstruction += (0xff & inst.constant);
                    break;

                case JType:
                    break;

                case RType:
                    //rs - [25:21]
                    curInstruction += (0x1f & inst.rt) << 21;

                    //rt - [20:16]
                    curInstruction += (0x1f & inst.rs) << 16;

                    //rd - [15:11]
                    curInstruction += (0x1f & inst.rd) << 11;

                    //shamt - [10:6] = 0

                    //function - [5:0]
                    curInstruction += (0x3f & getALU(inst.opcode));
                    break;
            }

            std::cout << "Hex Instruction: 0x" << std::hex << curInstruction << std::endl;

            mem->writeMemory(j, curInstruction);
            j++;
        }
        //ignore character
        else if(c == '\r');

        //add character to buffer
        else {
            buf[i] = c;
            i++;
        }

        c = fgetc(f);
    }
    fclose(f);
}


Instruction getInstruction(char* inst) {
    Instruction ins;

    //temporary character and pointer
    char temp[BUFFER_SIZE];
    int j = 0;

    //pointer to instruction character
    int i;
    
    //point in assembly instruction
    int point = 0;


    for(i=0; i<strlen(inst); i++) {

        //force read last character
        if(i == (strlen(inst) - 1)) {
            temp[j] = inst[i];
            j++;
            inst[i] = ' ';
        }

        //if end character then process buffer
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

        //ignore characters
        else if(inst[i] == ',');
        else if(inst[i] == '\r');
        else if(inst[i] == '\n');
        else if(inst[i] == '(');
        else if(inst[i] == ')');

        //add character to buffer
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
