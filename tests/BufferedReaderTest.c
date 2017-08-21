/*
 * File:   BufferedReaderTest.c
 * Author: Carlos Illobre
 *
 * Created on 12/03/2011, 03:23:50
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BufferedReader.h"
#include "BlockBufferedReader.h"


void testBufferedReaderOpenAndClose() {
    const char* FILENAME = "pepe.txt";
    const char* FILECONTENT = "jerryrice";
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), strlen(FILECONTENT)*sizeof(char), fp);
    fclose(fp);
    const size_t BUFFER_SIZE = 10;
    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, BUFFER_SIZE, 0);
    // El archivo debe estar abierto
    if (!reader.file) printf("%%TEST_FAILED%% time=0 testname=testBufferedReaderOpenAndClose (BufferedReaderTest) message=No se pudo abrir el archivo\n");
    // El buffer no debe ser NULL
    else if (!reader.buffer) printf("%%TEST_FAILED%% time=0 testname=testBufferedReaderOpenAndClose (BufferedReaderTest) message=No se creo el buffer\n");
    // El buffer debe tener el tamaño especificado
    else if (BUFFER_SIZE != reader.bufferSize) printf("%%TEST_FAILED%% time=0 testname=testBufferedReaderOpenAndClose (BufferedReaderTest) message=El tamaño del buffer no coincide\n");
    // Del archivo se tuvo que haber leido algo
    else if (reader.bufferLength == 0 || reader.bufferLength > reader.bufferSize) printf("%%TEST_FAILED%% time=0 testname=testBufferedReaderOpenAndClose (BufferedReaderTest) message=No se leyo nada del archivo\n");
    // El cursor debe apuntar al primer elemento del buffer
    else if (reader.cursor) printf("%%TEST_FAILED%% time=0 testname=testBufferedReaderOpenAndClose (BufferedReaderTest) message=El cursor no apunta al primer elemento del buffer\n");
    else BufferedReader_close(&reader);
    remove(FILENAME);
}

void testBufferedReaderOpenFileNotFound() {
    const char* FILENAME = "pepe.txt";
    remove(FILENAME);
    const size_t BUFFER_SIZE = 10;
    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, BUFFER_SIZE, 0);
    if (reader.file) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedReaderOpenFileNotFound (BufferedReaderTest) message=Se abrio un archivo que no existe\n");
        BufferedReader_close(&reader);
    }
}


void testReadImpar() {
    const char* FILENAME = "pepe.txt";
    char* FILECONTENT = "jerryrice";
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), strlen(FILECONTENT)*sizeof(char), fp);
    fclose(fp);
    const size_t BUFFER_SIZE = (size_t)(strlen(FILECONTENT)/2);
    char buffer[BUFFER_SIZE];
    char* cursor = FILECONTENT;
    int i;
    int eof = 0;
    BufferedReader reader;
    if (!BufferedReader_open(&reader, FILENAME, BUFFER_SIZE, 0)) {
        printf("%%TEST_FAILED%% time=0 testname=testReadImpar (BufferedReaderTest) message=No se pudo abrir el archivo\n");
    }
    else if (reader.bufferLength == 0) printf("%%TEST_FAILED%% time=0 testname=testReadImpar (BufferedReaderTest) message=No se pudo leer nada\n");
    else while (!eof) {
        for (i=0; i<BUFFER_SIZE; i++) {
            eof = BufferedReader_readNextByte(&reader, buffer+i);
            if (eof) break;
        }
        if (strncmp(cursor, buffer, i)) {
            printf("%%TEST_FAILED%% time=0 testname=testReadImpar (BufferedReaderTest) message=No se leyo bien\n");
            break;
        }
        cursor += i;
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
}

void testReadPar() {
    const char* FILENAME = "pepe.txt";
    char* FILECONTENT = "jerryricee";
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), strlen(FILECONTENT)*sizeof(char), fp);
    fclose(fp);
    const size_t BUFFER_SIZE = (size_t)(strlen(FILECONTENT)/2);
    char buffer[BUFFER_SIZE];
    char* cursor = FILECONTENT;
    int i;
    int eof = 0;
    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, BUFFER_SIZE, 0);
    if (!reader.file) printf("%%TEST_FAILED%% time=0 testname=testReadImpar (BufferedReaderTest) message=No se pudo abrir el archivo\n");
    else if (reader.bufferLength == 0) printf("%%TEST_FAILED%% time=0 testname=testReadImpar (BufferedReaderTest) message=No se pudo leer nada\n");
    else while (!eof) {
        for (i=0; i<BUFFER_SIZE; i++) {
            eof = BufferedReader_readNextByte(&reader, buffer+i);
            if (eof) break;
        }
        if (strncmp(cursor, buffer, i)) {
            printf("%%TEST_FAILED%% time=0 testname=testReadImpar (BufferedReaderTest) message=No se leyo bien\n");
            break;
        }
        cursor += i;
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
}


void testRead() {
    const char* FILENAME = "pepe.txt";
    unsigned char FILECONTENT[] = {1, 1, 2, 2, 1, 2};
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), 6*sizeof(unsigned char), fp);
    fclose(fp);

    unsigned char c;
    int i;

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, 4, 0);
    if (!reader.file) printf("%%TEST_FAILED%% time=0 testname=testRead (BufferedReaderTest) message=No se pudo abrir el archivo\n");
    else if (reader.bufferLength == 0) printf("%%TEST_FAILED%% time=0 testname=testReadImpar (BufferedReaderTest) message=No se pudo leer nada\n");
    else for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (c != FILECONTENT[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testRead (BufferedReaderTest) message=%d != %d\n", c, FILECONTENT[i]);
            break;
        } else {
            printf("%d) %d == %d\n", i, c, FILECONTENT[i]);
        }
    }
    if (i != 6) {
        printf("%%TEST_FAILED%% time=0 testname=testRead (BufferedReaderTest) message=Se leyeron %d caracteres en lugar de %d.\n", i, 6);
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
}


void testRead2() {
    const char* FILENAME = "pepe.txt";
    unsigned char FILECONTENT[] = {0, 1, 1, 0, 2, 1, 0, 1, 1, 1};
    size_t length = 10;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), length*sizeof(unsigned char), fp);
    fclose(fp);

    unsigned char c;
    int i;

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, 2048, 0);
    if (!reader.file) printf("%%TEST_FAILED%% time=0 testname=testRead2 (BufferedReaderTest) message=No se pudo abrir el archivo\n");
    else if (reader.bufferLength == 0) printf("%%TEST_FAILED%% time=0 testname=testReadImpar (BufferedReaderTest) message=No se pudo leer nada\n");
    else for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (c != FILECONTENT[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testRead2 (BufferedReaderTest) message=%d != %d\n", c, FILECONTENT[i]);
            break;
        } else {
            printf("%d) %d == %d\n", i, c, FILECONTENT[i]);
        }
    }
    if (i != length) {
        printf("%%TEST_FAILED%% time=0 testname=testRead2 (BufferedReaderTest) message=Se leyeron %d caracteres en lugar de %d.\n", i, length);
    }
    for (i=0; i<10; i++) {
        if (BufferedReader_readNextByte(&reader, &c)) {
            printf("%%TEST_FAILED%% time=0 testname=testRead2 (BufferedReaderTest) message=No da eof en la %d iteracion\n", i);
        }
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
}


void testBlockBufferedReader_open() {
    const char* FILENAME = "pepe.txt";
    const char* FILECONTENT = "abracadab";
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), strlen(FILECONTENT)*sizeof(char), fp);
    fclose(fp);
    const size_t BUFFER_SIZE = strlen(FILECONTENT);
    const size_t WINDOW_SIZE = 4;
    BlockBufferedReader this;
    int result = BlockBufferedReader_open(&this, FILENAME, BUFFER_SIZE + WINDOW_SIZE, WINDOW_SIZE);
    if(!result) {
        printf("%%TEST_FAILED%% time=0 testname=testBlockBufferedReader_open (BlockBufferedReaderTest) message=No se abrio el archivo\n");
        return;
    }
    if(this.bufferSize != BUFFER_SIZE+WINDOW_SIZE) {
        printf("%%TEST_FAILED%% time=0 testname=testBlockBufferedReader_open (BlockBufferedReaderTest) message=La matriz tiene tamaño %d cuando deberia tener tamaño %d\n", (int)this.bufferSize, (int)WINDOW_SIZE);
        return;
    }
    if(this.windowSize != WINDOW_SIZE) {
        printf("%%TEST_FAILED%% time=0 testname=testBlockBufferedReader_open (BlockBufferedReaderTest) message=La matriz tiene tamaño %d cuando deberia tener tamaño %d\n", (int)this.windowSize, (int)WINDOW_SIZE);
        return;
    }
    if(this.cursor != WINDOW_SIZE) {
        printf("%%TEST_FAILED%% time=0 testname=testBlockBufferedReader_open (BlockBufferedReaderTest) message=La matriz tiene tamaño %d cuando deberia tener tamaño %d\n", (int)this.windowSize, (int)WINDOW_SIZE);
        return;
    }
    if(!this.buffer) {
        printf("%%TEST_FAILED%% time=0 testname=testBlockBufferedReader_open (BlockBufferedReaderTest) message=No se creo el buffer\n");
        return;
    }
    if(!this.file) {
        printf("%%TEST_FAILED%% time=0 testname=testBlockBufferedReader_open (BlockBufferedReaderTest) message=No se creo el buffer\n");
        return;
    }
    BlockBufferedReader_close(&this);
    if(this.buffer) {
        printf("%%TEST_FAILED%% time=0 testname=testBlockBufferedReader_open (BlockBufferedReaderTest) message=No se cerro el buffer\n");
        return;
    }
    if(this.file) {
        printf("%%TEST_FAILED%% time=0 testname=testBlockBufferedReader_open (BlockBufferedReaderTest) message=No se cerro el buffer\n");
        return;
    }

}


int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% BufferedReaderTest\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testBufferedReaderOpenAndClose (BufferedReaderTest)\n");
    testBufferedReaderOpenAndClose();
    printf("%%TEST_FINISHED%% time=0 testBufferedReaderOpenAndClose (BufferedReaderTest) \n");

    printf("%%TEST_STARTED%% testBufferedReaderOpenFileNotFound (BufferedReaderTest)\n");
    testBufferedReaderOpenFileNotFound();
    printf("%%TEST_FINISHED%% time=0 testBufferedReaderOpenFileNotFound (BufferedReaderTest) \n");

    printf("%%TEST_STARTED%% testReadImpar (BufferedReaderTest)\n");
    testReadImpar();
    printf("%%TEST_FINISHED%% time=0 testReadImpar (BufferedReaderTest) \n");

    printf("%%TEST_STARTED%% testReadPar (BufferedReaderTest)\n");
    testReadPar();
    printf("%%TEST_FINISHED%% time=0 testReadPar (BufferedReaderTest) \n");

    printf("%%TEST_STARTED%% testRead (BufferedReaderTest)\n");
    testRead();
    printf("%%TEST_FINISHED%% time=0 testRead (BufferedReaderTest) \n");

    printf("%%TEST_STARTED%% testRead2 (BufferedReaderTest)\n");
    testRead2();
    printf("%%TEST_FINISHED%% time=0 testRead2 (BufferedReaderTest) \n");

    printf("%%TEST_STARTED%% testBlockBufferedReader_open (BlockBufferedReaderTest)\n");
    testBlockBufferedReader_open();
    printf("%%TEST_FINISHED%% time=0 testBlockBufferedReader_open (BlockBufferedReaderTest) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
