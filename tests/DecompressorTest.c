/*
 * File:   DecompressorTest.c
 * Author: Carlos Illobre
 *
 * Created on 03/06/2011, 21:56:17
 */

#include <stdio.h>
#include <stdlib.h>
#include "BufferedReader.h"
#include "Decompressor.h"
#include "Compressor.h"

/*
 * Simple C Test Suite
 */

void testDecompressor_lz77() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_DECOMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {0, 1, 1, 0, 2, 1, 0, 1, 1, 1};
    unsigned char decode[] = {1, 1, 2, 2, 1, 2};
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), 10, fp);
    fclose(fp);

    Decompressor_lz77(FILENAME, FILENAME_DECOMPR, 3, 3);

    unsigned char c;
    int i;
    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_DECOMPR, 4, 0);
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (c != decode[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testDecompressor_lz77 (DecompressorTest) message=i=%d) leido:%d != deberiaSer:%d\n", i, c, decode[i]);
        } else {
            //printf("%d) %d == %d\n", i, c, decode[i]);
        }
    }
    if (i != 6) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_lz77 (DecompressorTest) message=Se leyeron %d caracteres en lugar de %d.\n", i, 6);
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_DECOMPR);
}


void testDecompressor_lz77AAAAA() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_DECOMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {0, 1, 4, 0};
    unsigned char decode[] = {1, 1, 1, 1, 1};
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), 4, fp);
    fclose(fp);

    Decompressor_lz77(FILENAME, FILENAME_DECOMPR, 3, 4);

    unsigned char c;
    int i;
    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_DECOMPR, 4, 0);
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (c != decode[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testDecompressor_lz77AAAAA (DecompressorTest) message=i=%d) leido:%d != deberiaSer:%d\n", i, c, decode[i]);
        } else {
            //printf("%d) %d == %d\n", i, c, decode[i]);
        }
    }
    if (i != 5) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_lz77AAAAA (DecompressorTest) message=Se leyeron %d caracteres en lugar de %d.\n", i, 5);
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_DECOMPR);
}


void testDecompressor_lz77AAAABBBB() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_DECOMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {0, 1, 3, 0, 2, 3, 0};
    unsigned char decode[] = {1, 1, 1, 1, 2, 2, 2, 2};
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), 7, fp);
    fclose(fp);

    Decompressor_lz77(FILENAME, FILENAME_DECOMPR, 3, 4);

    unsigned char c;
    int i;
    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_DECOMPR, 4, 0);
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (c != decode[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testDecompressor_lz77AAAAA (DecompressorTest) message=i=%d) leido:%d != deberiaSer:%d\n", i, c, decode[i]);
        } else {
            //printf("%d) %d == %d\n", i, c, decode[i]);
        }
    }
    if (i != 8) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_lz77AAAAA (DecompressorTest) message=Se leyeron %d caracteres en lugar de %d.\n", i, 8);
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_DECOMPR);
}


void testDecompressor_lz77ABCDABCD() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_DECOMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {0, 1, 0, 2, 0, 3, 0, 4, 4, 3};
    unsigned char decode[] = {1, 2, 3, 4, 1, 2, 3, 4};
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), 10, fp);
    fclose(fp);

    Decompressor_lz77(FILENAME, FILENAME_DECOMPR, 3, 4);

    unsigned char c;
    int i;
    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_DECOMPR, 4, 0);
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (c != decode[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testDecompressor_lz77ABCDABCD (DecompressorTest) message=i=%d) leido:%d != deberiaSer:%d\n", i, c, decode[i]);
        } else {
            //printf("%d) %d == %d\n", i, c, decode[i]);
        }
    }
    if (i != 8) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_lz77ABCDABCD (DecompressorTest) message=Se leyeron %d caracteres en lugar de %d.\n", i, 8);
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_DECOMPR);
}


void testDecompressor_lzss() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt.XX";
    const char* FILENAME_DECOMPR = "pepe.txt";
    unsigned char FILECONTENT[] = {0, 192, 64, 0, 80, 16, 8, 16, 20};
    int filecontentSize = 9;
    unsigned char decode[] = {1, 1, 2, 2, 1, 2};
    int decodeSize = 6;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), filecontentSize, fp);
    fclose(fp);

    Decompressor_lzss(FILENAME, FILENAME_DECOMPR, 3, 4);

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_DECOMPR, 4, 0);

    unsigned char c;
    int i;
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (decode[i] != c) break;
    }
    if(i != decodeSize) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_lzss (DecompressorTest) message=lzss[%d]=%d buffer[%d]=%d\n", i, decode[i], i, reader.buffer[i]);
        return;
    }

    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_DECOMPR);
}


void testDecompressor_lzssAAAAA() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_DECOMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {0, 193, 0, 32};
    int filecontentSize = 4;
    unsigned char decode[] = {1, 1, 1, 1, 1};
    int decodeSize = 5;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), filecontentSize, fp);
    fclose(fp);

    Decompressor_lzss(FILENAME, FILENAME_DECOMPR, 3, 4);

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_DECOMPR, 4, 0);

    unsigned char c;
    int i;
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (decode[i] != c) break;
    }
    if(i != decodeSize) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_lzss (DecompressorTest) message=lzss[%d]=%d buffer[%d]=%d\n", i, decode[i], i, reader.buffer[i]);
        return;
    }

    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_DECOMPR);
}


void testDecompressor_lzssAAAAABBBB() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_DECOMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {0, 192, 192, 0, 80, 48, 8};
    int filecontentSize = 7;
    unsigned char decode[] = {1, 1, 1, 1, 2, 2, 2, 2};
    int decodeSize = 8;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), filecontentSize, fp);
    fclose(fp);

    Decompressor_lzss(FILENAME, FILENAME_DECOMPR, 3, 4);

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_DECOMPR, 4, 0);

    unsigned char c;
    int i;
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (decode[i] != c) break;
    }
    if(i != decodeSize) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_lzssAAAAABBBB (DecompressorTest) message=lzss[%d]=%d buffer[%d]=%d\n", i, decode[i], i, reader.buffer[i]);
        return;
    }

    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_DECOMPR);
}


void testDecompressor_lzssABCDABCD() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_DECOMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {0, 128, 128, 96, 72, 32, 28};
    int filecontentSize = 7;
    unsigned char decode[] = {1, 2, 3, 4, 1, 2, 3, 4};
    int decodeSize = 8;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), filecontentSize, fp);
    fclose(fp);

    Decompressor_lzss(FILENAME, FILENAME_DECOMPR, 3, 4);

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_DECOMPR, 4, 0);

    unsigned char c;
    int i;
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (decode[i] != c) break;
    }
    if(i != decodeSize) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_lzssABCDABCD (DecompressorTest) message=lzss[%d]=%d buffer[%d]=%d\n", i, decode[i], i, reader.buffer[i]);
        return;
    }

    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_DECOMPR);
}


void testDecompressor_blockSorting() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {5, 0, 3, 0, 1, 0, 2, 0, 4, 6, 0};
    size_t n = 11;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), n, fp);
    fclose(fp);

    Compressor_blockSorting(FILENAME, FILENAME_COMPR, n);
    remove(FILENAME);
    Decompressor_blockSorting(FILENAME_COMPR, FILENAME, n);

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, 4, 0);

    unsigned char c;
    int i;
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (FILECONTENT[i] != c) break;
    }
    if(i != n) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_blockSorting (DecompressorTest) message=lzss[%d]=%d buffer[%d]=%d\n", i, FILECONTENT[i], i, reader.buffer[i]);
        return;
    }

    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testDecompressor_blockSortingN4() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {5, 0, 3, 0, 1, 0, 2, 0, 4, 6, 0};
    size_t size = 11;
    size_t n = 4;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), size, fp);
    fclose(fp);

    Compressor_blockSorting(FILENAME, FILENAME_COMPR, n);
    remove(FILENAME);
    Decompressor_blockSorting(FILENAME_COMPR, FILENAME, n);

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, 4, 0);

    unsigned char c;
    int i;
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (FILECONTENT[i] != c) break;
    }
    if(i != size) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_blockSortingN4 (DecompressorTest) message=lzss[%d]=%d buffer[%d]=%d\n", i, FILECONTENT[i], i, reader.buffer[i]);
        return;
    }

    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testDecompressor_blockSortingN5() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {5, 0, 3, 0, 1, 0, 2, 0, 4, 6, 0};
    size_t size = 11;
    size_t n = 5;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), size, fp);
    fclose(fp);

    Compressor_blockSorting(FILENAME, FILENAME_COMPR, n);
    remove(FILENAME);
    Decompressor_blockSorting(FILENAME_COMPR, FILENAME, n);

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, 4, 0);

    unsigned char c;
    int i;
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (FILECONTENT[i] != c) break;
    }
    if(i != size) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_blockSortingN5 (DecompressorTest) message=lzss[%d]=%d buffer[%d]=%d\n", i, FILECONTENT[i], i, reader.buffer[i]);
        return;
    }

    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testDecompressor_blockSortingN5Par() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {5, 0, 3, 0, 1, 0, 2, 0, 4, 6};
    size_t size = 10;
    size_t n = 5;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), size, fp);
    fclose(fp);

    Compressor_blockSorting(FILENAME, FILENAME_COMPR, n);
    remove(FILENAME);
    Decompressor_blockSorting(FILENAME_COMPR, FILENAME, n);

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, 4, 0);

    unsigned char c;
    int i;
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (FILECONTENT[i] != c) break;
    }
    if(i != size) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_blockSortingN5Par (DecompressorTest) message=lzss[%d]=%d buffer[%d]=%d\n", i, FILECONTENT[i], i, reader.buffer[i]);
        return;
    }

    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testDecompressor_blockSortingMoveToFrontN4() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {5, 0, 3, 0, 1, 0, 2, 0, 4, 6, 0};
    size_t size = 11;
    size_t n = 4;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), size, fp);
    fclose(fp);

    Compressor_blockSortingMoveToFront(FILENAME, FILENAME_COMPR, n);
    remove(FILENAME);
    Decompressor_blockSortingMoveToFront(FILENAME_COMPR, FILENAME, n);

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, 4, 0);

    unsigned char c;
    int i;
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (FILECONTENT[i] != c) break;
    }
    if(i != size) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_blockSortingMoveToFrontN4 (DecompressorTest) message=lzss[%d]=%d buffer[%d]=%d\n", i, FILECONTENT[i], i, reader.buffer[i]);
        return;
    }

    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testDecompressor_blockSortingMoveToFrontAdaptativeHuffmanN4() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {5, 0, 3, 0, 1, 0, 2, 0, 4, 6, 0};
    size_t size = 11;
    size_t n = 4;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), size, fp);
    fclose(fp);

    Compressor_blockSortingMoveToFrontAdaptativeHuffman(FILENAME, FILENAME_COMPR, n);
    remove(FILENAME);
    Decompressor_blockSortingMoveToFrontAdaptativeHuffman(FILENAME_COMPR, FILENAME, n);

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, 4, 0);

    unsigned char c;
    int i;
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (FILECONTENT[i] != c) break;
    }
    if(i != size) {
        printf("%%TEST_FAILED%% time=0 testname=testDecompressor_blockSortingMoveToFrontAdaptativeHuffmanN4 (DecompressorTest) message=lzss[%d]=%d buffer[%d]=%d\n", i, FILECONTENT[i], i, c);
        return;
    }

    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% DecompressorTest\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testDecompressor_lz77 (DecompressorTest)\n");
    testDecompressor_lz77();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_lz77 (DecompressorTest) \n");

    printf("%%TEST_STARTED%% testDecompressor_lz77AAAAA (DecompressorTest)\n");
    testDecompressor_lz77AAAAA();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_lz77AAAAA (DecompressorTest) \n");

    printf("%%TEST_STARTED%% testDecompressor_lz77AAAABBBB (DecompressorTest)\n");
    testDecompressor_lz77AAAABBBB();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_lz77AAAABBBB (DecompressorTest) \n");

    printf("%%TEST_STARTED%% testDecompressor_lz77ABCDABCD (DecompressorTest)\n");
    testDecompressor_lz77ABCDABCD();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_lz77ABCDABCD (DecompressorTest) \n");

    printf("%%TEST_STARTED%% testDecompressor_lzss (DecompressorTest)\n");
    testDecompressor_lzss();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_lzss (DecompressorTest) \n");

    printf("%%TEST_STARTED%% testDecompressor_lzssAAAAA (DecompressorTest)\n");
    testDecompressor_lzssAAAAA();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_lzssAAAAA (DecompressorTest) \n");

    printf("%%TEST_STARTED%% testDecompressor_lzssAAAAABBBB (DecompressorTest)\n");
    testDecompressor_lzssAAAAABBBB();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_lzssAAAAABBBB (DecompressorTest) \n");

    printf("%%TEST_STARTED%% testDecompressor_lzssABCDABCD (DecompressorTest)\n");
    testDecompressor_lzssABCDABCD();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_lzssABCDABCD (DecompressorTest) \n");

    printf("%%TEST_STARTED%% testDecompressor_blockSorting (DecompressorTest)\n");
    testDecompressor_blockSorting();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_blockSorting (DecompressorTest) \n");

    printf("%%TEST_STARTED%% testDecompressor_blockSortingN4 (DecompressorTest)\n");
    testDecompressor_blockSortingN4();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_blockSortingN4 (DecompressorTest) \n");

    printf("%%TEST_STARTED%% testDecompressor_blockSortingN5 (DecompressorTest)\n");
    testDecompressor_blockSortingN5();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_blockSortingN5 (DecompressorTest) \n");

    printf("%%TEST_STARTED%% testDecompressor_blockSortingN5Par (DecompressorTest)\n");
    testDecompressor_blockSortingN5Par();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_blockSortingN5Par (DecompressorTest) \n");
    
    printf("%%TEST_STARTED%% testDecompressor_blockSortingMoveToFrontN4 (DecompressorTest)\n");
    testDecompressor_blockSortingMoveToFrontN4();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_blockSortingMoveToFrontN4 (DecompressorTest) \n");

    printf("%%TEST_STARTED%% testDecompressor_blockSortingMoveToFrontAdaptativeHuffmanN4 (DecompressorTest)\n");
    testDecompressor_blockSortingMoveToFrontAdaptativeHuffmanN4();
    printf("%%TEST_FINISHED%% time=0 testDecompressor_blockSortingMoveToFrontAdaptativeHuffmanN4 (DecompressorTest) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
