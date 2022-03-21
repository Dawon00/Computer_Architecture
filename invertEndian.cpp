#include "stdio.h"


//change endian
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

int main()
{
    FILE* input_f = NULL;
    FILE* output_f = NULL;
    int count;
    unsigned int data1;
    unsigned int data2;
    unsigned int size = 0x11223344;
    unsigned int temp = 0; //XOR연산할 때 필요한 임시변수


    // open a file to read data
    fopen_s(&input_f, "C:\\Users\\da692\\source\\repos\\컴구 2\\input.bin", "rb");

    // open a file to write data
    fopen_s(&output_f, "C:\\Users\\da692\\source\\repos\\컴구 2\\output.bin", "wb");

    while (1) {
        count = fread(&data1, sizeof(size), 1, input_f);

        if (count != 1)
            break;
        data2 = invertEndian(data1);
        //printf("big endian : %08X\n", data2);
        //printf("little endian : %08X\n", data1);

        // XOR연산하기
        //printf("%08X  ^  %08X\n", temp, data1);
        temp ^= data1;
        //printf("XOR한 결과 : %08X\n\n", temp);

        fwrite(&data2, sizeof(size), 1, output_f); // output.bin에 데이터 쓰기
    }

    printf("XOR한 최종 데이터 : %08X\n", temp); //XOR연산한 데이터

    fclose(input_f);
    fclose(output_f);

    return 0;
}

