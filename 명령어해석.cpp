#include <stdio.h>
#include <string.h>
#include <math.h>
#include <typeinfo>
#include <stdlib.h>

const int M_SIZE = 1024;
unsigned char MEM[M_SIZE]; //메모리 모델링. input file의 명령어 모두 저장
unsigned int IR;
int addr;

unsigned int invertEndian(unsigned int inVal) {
    int result;
    unsigned char bytes[4];
    bytes[0] = (unsigned char)((inVal >> 24) & 0xff);
    bytes[1] = (unsigned char)((inVal >> 16) & 0xff);
    bytes[2] = (unsigned char)((inVal >> 8) & 0xff);
    bytes[3] = (unsigned char)((inVal >> 0) & 0xff);

    result = ((int)bytes[0] << 0) |
        ((int)bytes[1] << 8) |
        ((int)bytes[2] << 16) |
        ((int)bytes[3] << 24);

    return result;
}


unsigned int memoryRead(unsigned int addr)
{
    union instructionRegister {
        unsigned int I;  	// 32-bit instruction
        struct RFormat {	// instruction R format
            unsigned int opcode : 6;
            unsigned int rs : 5;
            unsigned int rt : 5;
            unsigned int rd : 5;
            unsigned int funct : 6;
        }RI;
    }IR;
    for (int i = 0; i < addr; i +=4) {
        //MEM[i]<<24 , (MEM[i + 1] << 16) , (MEM[i + 2] << 8)), MEM[i + 3] 각각은 3C, 0A, 10, 00
        unsigned int instruction = (MEM[i] << 24) | (MEM[i + 1] << 16) | (MEM[i + 2] << 8) | MEM[i + 3];
        IR.RI.opcode = instruction >> 26;
        printf("Opc: %x, ", IR.RI.opcode);//00000F -> binary 001111
        /*IR.RI.rs = instruction >> 21; 
        printf("rs는 %05X\n", IR.RI.rs);
        IR.RI.rt = instruction >> 16; //00000A -> binary 01010
        printf("rt는 %05X\n", IR.RI.rt);
        IR.RI.rd = instruction >> 11; //000002 -> binary 00010
        printf("rd는 %05X\n", IR.RI.rd);*/
        IR.RI.funct = instruction; //000000 ->binary 000000
        printf("Fct: %x, ", IR.RI.funct);

    if (IR.RI.opcode == 0x0) {
        if (IR.RI.funct == 0x0)
            printf("Inst: sll\n");
        else if (IR.RI.funct == 0x2)
            printf("Inst: srl\n");
        else if (IR.RI.funct == 0x3)
            printf("Inst: sra\n");
        else if (IR.RI.funct == 0x8)
            printf("Inst: jr\n");
        else if (IR.RI.funct == 0xc)
            printf("Inst: syscall\n");
        else if (IR.RI.funct == 0x10)
            printf("Inst: mfhi\n");
        else if (IR.RI.funct == 0x12)
            printf("Inst: mflo\n");
        else if (IR.RI.funct == 0x18)
            printf("Inst: mul\n");
        else if (IR.RI.funct == 0x20)
            printf("Inst: add\n");
        else if (IR.RI.funct == 0x22)
            printf("Inst: sub\n");
        else if (IR.RI.funct == 0x24)
            printf("Inst: and\n");
        else if (IR.RI.funct == 0x25)
            printf("Inst: or\n");
        else if (IR.RI.funct == 0x26)
            printf("Inst: xor\n");
        else if (IR.RI.funct == 0x27)
            printf("Inst: nor\n");
        else if (IR.RI.funct == 0x2a)
            printf("Inst: slt\n");
        }
    else if (IR.RI.opcode == 0x1)
        printf("Inst: bltz\n");
    else if (IR.RI.opcode == 0x2)
        printf("Inst: j\n");
    else if (IR.RI.opcode == 0x3)
        printf("Inst: jal\n");
    else if (IR.RI.opcode == 0x4)
        printf("Inst: beq\n");
    else if (IR.RI.opcode == 0x5)
        printf("Inst: bne\n");
    else if (IR.RI.opcode == 0x8)
        printf("Inst: addi\n");
    else if (IR.RI.opcode == 0xa)
        printf("Inst: slti\n");
    else if (IR.RI.opcode == 0xc)
        printf("Inst: andi\n");
    else if (IR.RI.opcode == 0xd)
        printf("Inst: ori\n");
    else if (IR.RI.opcode == 0xe)
        printf("Inst: xori\n");
    else if (IR.RI.opcode == 0xf)
        printf("Inst: lui\n");
    else if (IR.RI.opcode == 0x20)
        printf("Inst: lb\n");
    else if (IR.RI.opcode == 0x23)
        printf("Inst: lw\n");
    else if (IR.RI.opcode == 0x24)
        printf("Inst: lbu\n");
    else if (IR.RI.opcode == 0x28)
        printf("Inst: sb\n");
    else if (IR.RI.opcode == 0x2b)
        printf("Inst: sw\n");
    }
    
    return 0;
}
void memoryWrite(int addr, unsigned int data)
{

    for (int i = 3; i >= 0; i--) {
        unsigned char one_byte;
        one_byte = (unsigned char)((data >> i * 8) & 0x000000FF);
        MEM[addr] = (unsigned char)one_byte; //MEM[8]: 0xAA, MEM[9]: 0xBB...
        addr++;
    }

}


int main()
{
    FILE* pFile = NULL;
    errno_t err;
    int count;
    unsigned int data1;
    unsigned int data2;
    unsigned int size = 0x11223344;
    unsigned int num_instruction = 0;//number of instructions
    unsigned int num_data = 0; //number of data
    unsigned int flag = 1;
    int instruct_count = 0;


    err = fopen_s(&pFile, "C:\\Users\\da692\\Desktop\\자료\\3학년 1학기\\컴퓨터구조\\과제3\\machine_example\\as_ex04_fct.bin", "rb");
    if (err) {
        printf("Cannot open file\n");
        return 1;
    }
    // 1. read given executable file and extract
  //    information and write the instructions
  //    to memory 지정된 실행파일을 읽고 정보를 추출 -> 메모리에 명령 작성
    while (1) {
        count = fread(&data1, sizeof(size), 1, pFile);

        if (count != 1)
            break;
        //printf("%08X\n", data1);
        data2 = invertEndian(data1);
        //printf("%08X\n", data2);
        if (flag == 1 && num_instruction == 0) {
            num_instruction = data2; //4번째 바이트까지 명령어개수에 저장
            flag = 0;
        }
        else if (flag == 0) {
            //printf("data2는 %08X\n", data2);
            num_data = data2;
            flag = 1;
        }
        else if (flag == 1) {
            //명령어 개수만큼 메모리에 저장
            memoryWrite(addr, data2);
            addr += 4;
            instruct_count++;
            if (num_instruction == instruct_count) {
                break;
            }
        }
    }

    // 2. read instructions and decode
    //    then, print the instruction names

    printf("Number of Instructions: %d, Number of Data: %d\n", num_instruction, num_data);

    memoryRead(addr);
    fclose(pFile);
    printf("\n\n");

    return 0;
}



