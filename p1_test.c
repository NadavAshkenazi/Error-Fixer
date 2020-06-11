#include <stdio.h>
#include <stdint.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define DATA_CHUNK_SIZE 8

bool isCorrect(uint8_t data);
int find_index_of_errored_row(uint8_t* data, int* err_row_index);
int fix_data_block(uint8_t* data);

bool correctnessCheck(uint8_t byte) {
    uint8_t cur;
    int count = 0;
    for (int i = 0; i < 8; i++){
        cur = (byte >> i) & 1; 
        if (cur == 1) count++;
    }
    if (count%2 == 0) return true;
    else return false;
}

bool cmpDataChunks(uint8_t* data1, uint8_t* data2) {
    for (int i = 0; i <DATA_CHUNK_SIZE; i++) {
        if (data1[i] != data2[i]) return false;
    }
    return true;
}

int main() {
    uint8_t rand_data[DATA_CHUNK_SIZE] = {
        0b00000000,
        0b11111111,
        0b11110000,
        0b00001111,
        0b10000000,
        0b11110111,
        0b01110101,
        0b11000111    
    };
    
    uint8_t rand_data_copy[DATA_CHUNK_SIZE] = {
        0b00000000,
        0b11111111,
        0b11110000,
        0b00001111,
        0b10000000,
        0b11110111,
        0b01110101,
        0b11000111    
    };

    uint8_t data2_ok[DATA_CHUNK_SIZE] = {
        0b01010000,
        0b01001101,
        0b10110010,
        0b11001010,
        0b10000100,
        0b11010111,
        0b01010101,
        0b01100011     
    };
    
    uint8_t data2_bad[DATA_CHUNK_SIZE] = {
        0b01010000,
        0b01001101,
        0b10110010,
        0b11001010,
        0b10100100,
        0b11010111,
        0b01010101,
        0b01100011    
    };

    uint8_t data2_worse[DATA_CHUNK_SIZE] = {
        0b01010000,
        0b00001101,
        0b10110010,
        0b11001010,
        0b10100100,
        0b11010111,
        0b01010101,
        0b01100011    
    };

    
    uint8_t data2_worst[DATA_CHUNK_SIZE] = {
        0b01010000,
        0b01001101,
        0b10110010,
        0b11001010,
        0b10100101,
        0b11010111,
        0b01010101,
        0b01100011    
    };

    uint8_t data3_ok[DATA_CHUNK_SIZE] = {
        0b10000111,
        0b10101100,
        0b10101100,
        0b11011101,
        0b10101010,
        0b01000001,
        0b11001100,
        0b01111101
    };

    uint8_t data3_bad[DATA_CHUNK_SIZE] = {
        0b10000111,
        0b10101100,
        0b10101100,
        0b11111101, // this is wrong
        0b10101010,
        0b01000001,
        0b11001100,
        0b01111101
    };

    uint8_t data3_worse[DATA_CHUNK_SIZE] = {
        0b10000111,
        0b10101100,
        0b10101100,
        0b11011101,
        0b10101010,
        0b11001001, // 2 changes here
        0b11001100,
        0b01111101
    };
    
    uint8_t data3_worst[DATA_CHUNK_SIZE] = {
        0b10000111,
        0b10101100,
        0b10101100,
        0b11011101,
        0b10101010,
        0b01000001,
        0b11011100, //one change in each of the last 2 lines
        0b01101101
    };

    for (int i = 0; i < DATA_CHUNK_SIZE; i++){
        assert(correctnessCheck(rand_data[i]) == isCorrect(rand_data[i]));
    }
    printf("----------------------------------PASSED 1-------------------------------\n");

    assert(fix_data_block(rand_data) == 0);
    assert(cmpDataChunks(rand_data, rand_data_copy));
    printf("----------------------------------PASSED 2-------------------------------\n");

    for (int i = 0; i < DATA_CHUNK_SIZE; i++){
        assert(correctnessCheck(data2_ok[i]) == isCorrect(data2_ok[i]));
    }
    printf("----------------------------------PASSED 3-------------------------------\n");
    int row;
    assert(find_index_of_errored_row(data2_ok, &row) == 0);
    assert (fix_data_block(data2_ok) == 1);
    assert(row == -1);
    
    assert(find_index_of_errored_row(data2_bad, &row) == 1);
    assert(row == 4);
    assert(fix_data_block(data2_bad) == 2);
    assert(cmpDataChunks(data2_bad,data2_ok));

    printf("----------------------------------PASSED 4-------------------------------\n");

    assert(fix_data_block(data2_worse) == 0);
    printf("----------------------------------PASSED 5-------------------------------\n");

    assert(fix_data_block(data2_worst) == 0);
    printf("----------------------------------PASSED 6-------------------------------\n");

    for (int i = 0; i < DATA_CHUNK_SIZE; i++){
        assert(correctnessCheck(data3_ok[i]) == isCorrect(data3_ok[i]));
        assert(correctnessCheck(data3_bad[i]) == isCorrect(data3_bad[i]));
        assert(correctnessCheck(data3_worse[i]) == isCorrect(data3_worse[i]));
        assert(correctnessCheck(data3_worst[i]) == isCorrect(data3_worst[i]));
    }
    printf("----------------------------------PASSED 7-------------------------------\n");

    assert(fix_data_block(data3_ok) == 1);
    assert(fix_data_block(data3_bad) == 2);
    assert(fix_data_block(data3_worse) == 0);
    assert(fix_data_block(data3_worst) == 0);

    printf("----------------------------------PASSED 8-------------------------------\n");

    return 0;
}