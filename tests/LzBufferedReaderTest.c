/*
 * File:   LzBufferedReaderTest.c
 * Author: Carlos Illobre
 *
 * Created on 16/04/2011, 13:32:53
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LzBufferedReader.h"

/*
 * Simple C Test Suite
 */

void testLzBufferedReader_ReadNext1() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILECONTENT = "ababacab";
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), strlen(FILECONTENT)*sizeof(char), fp);
    fclose(fp);

    LzBufferedReader this;
    LzBufferedReader_open(&this, FILENAME, 6, 2);
    size_t length, distance;
    unsigned char c;

    // Leo el primer caracter
    int result = LzBufferedReader_readNextLengthDistance(&this, &length, &distance, 1);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    // Longitud debe ser cero
    if (length) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=Longitud: %d\n", (int)length);
        return;
    }
    // Leo el caracter
    result = LzBufferedReader_readNextByte(&this, &c);
    // El caracter debe ser el primero
    if (c != 'a') {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=Caracter: %c\n", c);
        return;
    }


    result = LzBufferedReader_readNextLengthDistance(&this, &length, &distance, 1);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    // Longitud debe ser cero
    if (length) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=Longitud: %d\n", (int)length);
        return;
    }
    // Leo el caracter
    result = LzBufferedReader_readNextByte(&this, &c);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    // El caracter debe ser el primero
    if (c != 'b') {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=Caracter: %c\n", c);
        return;
    }

    result = LzBufferedReader_readNextLengthDistance(&this, &length, &distance, 1);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    if (distance != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=Posicion: %d\n", (int)distance);
        return;
    }
    if (length != 3) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=Longitud: %d\n", (int)length);
        return;
    }
    result = LzBufferedReader_readNextByte(&this, &c);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    if (c != 'c') {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=Caracter: %c\n", c);
        return;
    }

    result = LzBufferedReader_readNextLengthDistance(&this, &length, &distance, 1);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    if (distance != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=Posicion: %d\n", (int)distance);
        return;
    }
    if (length != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=Longitud: %d\n", (int)length);
        return;
    }
    result = LzBufferedReader_readNextByte(&this, &c);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    if (c != 'b') {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=Caracter: %c\n", c);
        return;
    }

    result = LzBufferedReader_readNextByte(&this, &c);
    if(result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) message=Tuvo que haberse llegado a eof\n");
        return;
    }

    LzBufferedReader_close(&this);
}


void testLzBufferedReader_ReadNext() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILECONTENT = "abbaababba";
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), strlen(FILECONTENT)*sizeof(char), fp);
    fclose(fp);

    LzBufferedReader this;
    LzBufferedReader_open(&this, FILENAME, 6, 2);
    size_t length, distance;
    unsigned char c;

    // Leo el primer caracter
    int result = LzBufferedReader_readNextLengthDistance(&this, &length, &distance, 1);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    // Longitud debe ser cero
    if (length) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Longitud: %d\n", (int)length);
        return;
    }
    // Leo el caracter
    result = LzBufferedReader_readNextByte(&this, &c);
    // El caracter debe ser el primero
    if (c != 'a') {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Caracter: %c\n", c);
        return;
    }


    result = LzBufferedReader_readNextLengthDistance(&this, &length, &distance, 1);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    // Longitud debe ser cero
    if (length) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Longitud: %d\n", (int)length);
        return;
    }
    // Leo el caracter
    result = LzBufferedReader_readNextByte(&this, &c);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    // El caracter debe ser el primero
    if (c != 'b') {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Caracter: %c\n", c);
        return;
    }

    result = LzBufferedReader_readNextLengthDistance(&this, &length, &distance, 1);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    if (distance != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Posicion: %d\n", (int)distance);
        return;
    }
    if (length != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Longitud: %d\n", (int)length);
        return;
    }
    result = LzBufferedReader_readNextByte(&this, &c);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    if (c != 'a') {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Caracter: %c\n", c);
        return;
    }

    result = LzBufferedReader_readNextLengthDistance(&this, &length, &distance, 1);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    if (distance != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Posicion: %d\n", (int)distance);
        return;
    }
    if (length != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Longitud: %d\n", (int)length);
        return;
    }
    result = LzBufferedReader_readNextByte(&this, &c);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    if (c != 'b') {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Caracter: %c\n", c);
        return;
    }

    result = LzBufferedReader_readNextLengthDistance(&this, &length, &distance, 1);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    if (distance != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Posicion: %d\n", (int)distance);
        return;
    }
    if (length != 2) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Longitud: %d\n", (int)length);
        return;
    }
    result = LzBufferedReader_readNextByte(&this, &c);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    if (c != 'b') {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Caracter: %c\n", c);
        return;
    }

    result = LzBufferedReader_readNextLengthDistance(&this, &length, &distance, 1);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    if (length != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Longitud: %d\n", (int)length);
        return;
    }
    result = LzBufferedReader_readNextByte(&this, &c);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=No se leyo el caracter\n");
        return;
    }
    if (c != 'a') {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Caracter: %c\n", c);
        return;
    }

    result = LzBufferedReader_readNextByte(&this, &c);
    if(result) {
        printf("%%TEST_FAILED%% time=0 testname=testLzBufferedReader_ReadNext (LzBufferedReaderTest) message=Tuvo que haberse llegado a eof\n");
        return;
    }

    LzBufferedReader_close(&this);
}


int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% LzBufferedReaderTest\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testLzBufferedReader_ReadNext1 (LzBufferedReaderTest)\n");
    testLzBufferedReader_ReadNext1();
    printf("%%TEST_FINISHED%% time=0 testLzBufferedReader_ReadNext1 (LzBufferedReaderTest) \n");

    printf("%%TEST_STARTED%% testLzBufferedReader_ReadNext (LzBufferedReaderTest)\n");
    testLzBufferedReader_ReadNext();
    printf("%%TEST_FINISHED%% time=0 testLzBufferedReader_ReadNext (LzBufferedReaderTest) \n");


    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
