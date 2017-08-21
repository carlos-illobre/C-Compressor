/*
 * File:   BlockSortingTest.c
 * Author: Carlos Illobre
 *
 * opend on 09/04/2011, 23:09:59
 */

#include <stdio.h>
#include <stdlib.h>
#include "BlockSorting.h"
#include <string.h>

/*
 * Simple C Test Suite
 */


void testBlockSorting_compareRotations() {
    const char* FILECONTENT = "abracadab";
    const size_t BUFFER_SIZE = strlen(FILECONTENT);
    size_t i = 2;
    size_t j = 6;
    int result = BlockSorting_compareRotations(FILECONTENT, i, j, BUFFER_SIZE);
    if(result <= 0) {
        printf("%%TEST_FAILED%% time=0 testname=testBlockSorting_compareRotations (BlockSortingTest) message=LA comparacion no dio negativo\n");
        return;
    }
}


void testBlockSorting_get() {
    unsigned char* matrix[] = {"abracadab",
                     "bracadaba",
                     "racadabab",
                     "acadababr",
                     "cadababra",
                     "adababrac",
                     "dababraca",
                     "ababracad",
                     "babracada"};
    const char* FILECONTENT = matrix[0];
    const size_t BUFFER_SIZE = strlen(matrix[0]);
    size_t i;
    size_t j;
    unsigned char result;
    for (i=0; i<BUFFER_SIZE; i++) {
        for (j=0; j<BUFFER_SIZE; j++) {
            result = BlockSorting_get(FILECONTENT, i, j, BUFFER_SIZE);
            if(result != matrix[i][j]) {
                printf("%%TEST_FAILED%% time=0 testname=testBlockSorting_get (BlockSortingTest) message=El caracter de la posicion (%d, %d) no coincide. Es %d y deberia ser %d\n", (int)i, (int)j, result, matrix[i][j]);
                return;
            }
        }
    }
}


void testBlockSorting_codeBlock() {
    const char* CODE = "dbrcaaaab";
    const size_t BUFFER_SIZE = strlen(CODE);
    char* FILECONTENT = (char*)malloc(BUFFER_SIZE * sizeof(unsigned char));
    FILECONTENT[0] = 'a';
    FILECONTENT[1] = 'b';
    FILECONTENT[2] = 'r';
    FILECONTENT[3] = 'a';
    FILECONTENT[4] = 'c';
    FILECONTENT[5] = 'a';
    FILECONTENT[6] = 'd';
    FILECONTENT[7] = 'a';
    FILECONTENT[8] = 'b';
    int i;
    BlockSorting_code(FILECONTENT, BUFFER_SIZE);
    for (i=0; i<BUFFER_SIZE; i++) {
        if(FILECONTENT[i] != CODE[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testBlockSorting_codeBlock (BlockSortingTest) message=FILECONTENT[%d]=%d != CODE[%d]=%d\n", i, FILECONTENT[i], i, CODE[i]);
            return;
        }
    }
    free(FILECONTENT);
}


void testBlockSorting_codeBlockIndex() {
    const unsigned char* CODE = "cbaa";
    size_t index = 2;
    const size_t BUFFER_SIZE = strlen(CODE);
    unsigned char* FILECONTENT = (unsigned char*)malloc(BUFFER_SIZE * sizeof(unsigned char));
    FILECONTENT[0] = 'b';
    FILECONTENT[1] = 'a';
    FILECONTENT[2] = 'c';
    FILECONTENT[3] = 'a';
    int i;
    size_t ii = BlockSorting_code(FILECONTENT, BUFFER_SIZE);
    for (i=0; i<BUFFER_SIZE; i++) {
        if(FILECONTENT[i] != CODE[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testBlockSorting_codeBlockIndex (BlockSortingTest) message=FILECONTENT[%d]=%d != CODE[%d]=%d\n", i, FILECONTENT[i], i, CODE[i]);
            return;
        }
    }
    if(ii != index) {
        printf("%%TEST_FAILED%% time=0 testname=testBlockSorting_codeBlockIndex (BlockSortingTest) message=Indice no coincide: %d != %d\n", (int)ii, (int)index);
    }
    free(FILECONTENT);
}


void testBlockSorting_decodeBlock() {
    const char* DECODE = "abracadab";
    const size_t BUFFER_SIZE = strlen(DECODE);
    char* FILECONTENT = (char*)malloc(BUFFER_SIZE * sizeof(unsigned char));
    FILECONTENT[0] = 'd';
    FILECONTENT[1] = 'b';
    FILECONTENT[2] = 'r';
    FILECONTENT[3] = 'c';
    FILECONTENT[4] = 'a';
    FILECONTENT[5] = 'a';
    FILECONTENT[6] = 'a';
    FILECONTENT[7] = 'a';
    FILECONTENT[8] = 'b';
    int i;
    BlockSorting_decode(FILECONTENT, 1, BUFFER_SIZE);
    for (i=0; i<BUFFER_SIZE; i++) {
        if(FILECONTENT[i] != DECODE[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testBlockSorting_decodeBlock (BlockSortingTest) message=FILECONTENT[%d]=%c != CODE[%d]=%c\n", i, FILECONTENT[i], i, DECODE[i]);
            return;
        }
    }
    free(FILECONTENT);
}


int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% BlockSortingTest\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%%  testBlockSorting_get (BlockSortingTest)\n");
    testBlockSorting_get();
    printf("%%TEST_FINISHED%% time=0 testBlockSorting_get (BlockSortingTest)\n");

    printf("%%TEST_STARTED%%  testBlockSorting_compareRotations (BlockSortingTest)\n");
    testBlockSorting_compareRotations();
    printf("%%TEST_FINISHED%% time=0 testBlockSorting_compareRotations (BlockSortingTest)\n");

    printf("%%TEST_STARTED%%  testBlockSorting_codeBlock (BlockSortingTest)\n");
    testBlockSorting_codeBlock();
    printf("%%TEST_FINISHED%% time=0 testBlockSorting_codeBlock (BlockSortingTest)\n");

    printf("%%TEST_STARTED%%  testBlockSorting_codeBlockIndex (BlockSortingTest)\n");
    testBlockSorting_codeBlockIndex();
    printf("%%TEST_FINISHED%% time=0 testBlockSorting_codeBlockIndex (BlockSortingTest)\n");

    printf("%%TEST_STARTED%%  testBlockSorting_decodeBlock (BlockSortingTest)\n");
    testBlockSorting_decodeBlock();
    printf("%%TEST_FINISHED%% time=0 testBlockSorting_decodeBlock (BlockSortingTest)\n");
    
    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
