/*
 * File:   MoveToFrontTest.c
 * Author: Carlos Illobre
 *
 * Created on 09/04/2011, 03:00:55
 */

#include <stdio.h>
#include <stdlib.h>
#include "MoveToFront.h"
#include <string.h>

/*
 * Simple C Test Suite
 */

void testMoveToFront() {
    unsigned char buffer[] = {'a', 'b', 'r', 'a', 'c', 'a', 'd', 'a', 'b'};
    size_t n = 9;
    unsigned char code[] = {'a', 'b', 'r', 2, 'd', 1, 'e', 1, 4};
    MoveToFront mtf;
    int result = MoveToFront_create(&mtf);
    MoveToFront_code(&mtf, buffer, n);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testMoveToFront (MoveToFrontTest) message=Retorno cero\n");
        return;
    }
    int i;
    for (i=0; i<n; i++) {
        if (code[i] != buffer[i]) break;
    }
    if(i != n) {
        printf("%%TEST_FAILED%% time=0 testname=testMoveToFront (MoveToFrontTest) message=mtf[%d]=%d buffer[%d]=%d\n", i, code[i], i, buffer[i]);
        return;
    }
    MoveToFront_destroy(&mtf);
}


void testMoveToFront_decode() {
    size_t n = 11;
    unsigned char code[] = {3, 2, 5, 4, 6, 5, 0, 0, 0, 0, 6};
    unsigned char decode[] = {3, 1, 5, 2, 6, 0, 0, 0, 0, 0, 4};
    MoveToFront mtf;
    int result = MoveToFront_create(&mtf);
    MoveToFront_decode(&mtf, code, n);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testMoveToFront_decode (MoveToFrontTest) message=Retorno cero\n");
        return;
    }
    int i;
    for (i=0; i<n; i++) {
        if (code[i] != decode[i]) break;
    }
    if(i != n) {
        printf("%%TEST_FAILED%% time=0 testname=testMoveToFront_decode (MoveToFrontTest) message=code[%d]=%d decode[%d]=%d\n", i, code[i], i, decode[i]);
        return;
    }
    MoveToFront_destroy(&mtf);
}


int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% MoveToFrontTest\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%%  testMoveToFront (MoveToFrontTest)\n");
    testMoveToFront();
    printf("%%TEST_FINISHED%% time=0 testMoveToFront (MoveToFrontTest)\n");

    printf("%%TEST_STARTED%%  testMoveToFront_decode (MoveToFrontTest)\n");
    testMoveToFront_decode();
    printf("%%TEST_FINISHED%% time=0 testMoveToFront_decode (MoveToFrontTest)\n");
    
    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
