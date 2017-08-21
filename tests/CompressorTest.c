/*
 * File:   CompressorTest.c
 * Author: Carlos Illobre
 *
 * Created on 25/03/2011, 04:30:05
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Compressor.h"
#include "Decompressor.h"
#include "BlockBufferedReader.h"

/*
 * Simple C Test Suite
 */

void testCompressor_compressDynamicHuffman() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    const unsigned char* FILECONTENT = "jerryrice";
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), 9, fp);
    fclose(fp);

    // comprimo el archivo de prueba
    int result = Compressor_adaptiveHuffman(FILENAME, FILENAME_COMPR, 4);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressDynamicHuffman (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }
 
    // Descomprimo el archivo de prueba
    if(Decompressor_adaptativeHuffman(FILENAME_COMPR, FILENAME, 3) != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressDynamicHuffman (CompressorTest) message=Error al descomprimir\n");
        return;
    }

    // Leo el archivo descomprimido
    unsigned char buffer[9];
    fp = fopen(FILENAME, "rb");
    int length = fread(buffer, sizeof(unsigned char), 9, fp);
    fclose(fp);

    if(length != 9) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressDynamicHuffman (CompressorTest) message=No coinciden las longitudes: %d != %d\n", length, (int)strlen(FILECONTENT));
        return;
    }
    // Comparo el archivo descomprimido con el original
    int i;
    for (i=0; i<length; i++) {
        if(FILECONTENT[i] != buffer[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressDynamicHuffman (CompressorTest) message=%d) El caracter %d del archivo descomprimido no coincide con el original %d\n", i, buffer[i], FILECONTENT[i]);
            return;
        }
    }
}


void testCompressor_compressLz77_AAAAA() {
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {1, 1, 1, 1, 1};
    int filecontentsize = 5;
    unsigned char code[] = {0, 1, 4, 0};
    int codesize = 4;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), filecontentsize, fp);
    fclose(fp);

    // comprimo el archivo de prueba
    int result = Compressor_lz77(FILENAME, FILENAME_COMPR, filecontentsize+1, filecontentsize);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_AAAAA (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }

    unsigned char c;
    int i;
    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_COMPR, codesize, 0);
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (c != code[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_AAAAA (DecompressorTest) message=i=%d) leido:%d != deberiaSer:%d\n", i, c, code[i]);
        } else {
            //printf("%d) %d == %d\n", i, c, decode[i]);
        }
    }
    if (i != codesize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_AAAAA (DecompressorTest) message=Se leyeron %d caracteres en lugar de %d.\n", i, codesize);
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testCompressor_compressLz77_500() {
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    int filecontentsize = 500;
    BufferedWriter* writer = BufferedWriter_open(FILENAME, 10);
    int i;
    for (i=0;i <filecontentsize; i++) {
        BufferedWriter_writeByte(writer, 1);
    }
    BufferedWriter_close(writer);

    unsigned char code[] = {0, 1, 255, filecontentsize-256, 0};
    int codesize = 5;

    // comprimo el archivo de prueba
    int result = Compressor_lz77(FILENAME, FILENAME_COMPR, 20, 10);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_500 (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }

    unsigned char c;

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_COMPR, codesize, 0);
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (c != code[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_500 (DecompressorTest) message=i=%d) leido:%d != deberiaSer:%d\n", i, c, code[i]);
        } else {
            //printf("%d) %d == %d\n", i, c, decode[i]);
        }
    }
    if (i != codesize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_500 (DecompressorTest) message=Se leyeron %d caracteres en lugar de %d.\n", i, codesize);
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testCompressor_compressLz77_1000() {
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    int filecontentsize = 1000;
    BufferedWriter* writer = BufferedWriter_open(FILENAME, 10);
    int i;
    for (i=0;i <filecontentsize; i++) {
        BufferedWriter_writeByte(writer, 1);
    }
    BufferedWriter_close(writer);

    unsigned char code[] = {0, 1, 255, 255, 255, filecontentsize-3*255-1, 0};
    int codesize = 7;

    // comprimo el archivo de prueba
    int result = Compressor_lz77(FILENAME, FILENAME_COMPR, 20, 10);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_1000 (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }

    unsigned char c;

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_COMPR, codesize, 0);
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (c != code[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_1000 (DecompressorTest) message=i=%d) leido:%d != deberiaSer:%d\n", i, c, code[i]);
        } else {
            //printf("%d) %d == %d\n", i, c, decode[i]);
        }
    }
    if (i != codesize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_1000 (DecompressorTest) message=Se leyeron %d caracteres en lugar de %d.\n", i, codesize);
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testCompressor_compressLz77_766() {
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    int filecontentsize = 766;
    BufferedWriter* writer = BufferedWriter_open(FILENAME, 10);
    int i;
    for (i=0;i <filecontentsize; i++) {
        BufferedWriter_writeByte(writer, 1);
    }
    BufferedWriter_close(writer);

    unsigned char code[] = {0, 1, 255, 255, 255, 0, 0};
    int codesize = 7;

    // comprimo el archivo de prueba
    int result = Compressor_lz77(FILENAME, FILENAME_COMPR, 20, 10);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_766 (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }

    unsigned char c;

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_COMPR, codesize, 0);
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (c != code[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_766 (DecompressorTest) message=i=%d) leido:%d != deberiaSer:%d\n", i, c, code[i]);
        } else {
            //printf("%d) %d == %d\n", i, c, decode[i]);
        }
    }
    if (i != codesize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_766 (DecompressorTest) message=Se leyeron %d caracteres en lugar de %d.\n", i, codesize);
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testCompressor_compressLz77_767() {
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    int filecontentsize = 767;
    BufferedWriter* writer = BufferedWriter_open(FILENAME, 10);
    int i;
    for (i=0;i <filecontentsize-1; i++) {
        BufferedWriter_writeByte(writer, 1);
    }
    BufferedWriter_writeByte(writer, 2);
    BufferedWriter_close(writer);

    unsigned char code[] = {0, 1, 255, 255, 255, 0, 0, 2};
    int codesize = 8;

    // comprimo el archivo de prueba
    int result = Compressor_lz77(FILENAME, FILENAME_COMPR, 20, 10);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_767 (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }

    unsigned char c;

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_COMPR, codesize, 0);
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (c != code[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_767 (DecompressorTest) message=i=%d) leido:%d != deberiaSer:%d\n", i, c, code[i]);
        } else {
            //printf("%d) %d == %d\n", i, c, decode[i]);
        }
    }
    if (i != codesize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_767 (DecompressorTest) message=Se leyeron %d caracteres en lugar de %d.\n", i, codesize);
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testCompressor_compressLz77_1000B1000() {
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    int filecontentsize = 1000;
    BufferedWriter* writer = BufferedWriter_open(FILENAME, 10);
    int i;
    // Escribo 1000 A
    for (i=0;i <filecontentsize; i++) {
        BufferedWriter_writeByte(writer, 1);
    }
    // Escribo 1 B
    BufferedWriter_writeByte(writer, 2);
    // Escribo 1000 A
    for (i=0;i <filecontentsize; i++) {
        BufferedWriter_writeByte(writer, 1);
    }
    BufferedWriter_close(writer);
    // 0A length(999) dist(0) B length(1000) dist(1000)
    unsigned char code[] = {0, 1, 255, 255, 255, filecontentsize-3*255-1, 0, 2, 255, 255, 255, filecontentsize-3*255, 255, 255, 255, filecontentsize-3*255};
    int codesize = 16;

    // comprimo el archivo de prueba
    int result = Compressor_lz77(FILENAME, FILENAME_COMPR, 2010+1200, 1200);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_1000B1000 (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }

    unsigned char c;

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_COMPR, codesize, 0);
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (c != code[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_1000B1000 (DecompressorTest) message=i=%d) leido:%d != deberiaSer:%d\n", i, c, code[i]);
        } else {
            //printf("%d) %d == %d\n", i, c, decode[i]);
        }
    }
    if (i != codesize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_1000B1000 (DecompressorTest) message=Se leyeron %d caracteres en lugar de %d.\n", i, codesize);
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testCompressor_compressLz77_766B766() {
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    int filecontentsize = 766;
    BufferedWriter* writer = BufferedWriter_open(FILENAME, 10);
    int i;
    // Escribo 766 A
    for (i=0;i <filecontentsize; i++) {
        BufferedWriter_writeByte(writer, 1);
    }
    // Escribo 1 B
    BufferedWriter_writeByte(writer, 2);
    // Escribo 765 A
    for (i=0;i <filecontentsize-1; i++) {
        BufferedWriter_writeByte(writer, 1);
    }
    BufferedWriter_close(writer);
    // 0A length(999) dist(0) B length(1000) dist(1000)
    unsigned char code[] = {0, 1, 255, 255, 255, 0, 0, 2, 255, 255, 255, 0, 255, 255, 255, 0};
    int codesize = 16;

    // comprimo el archivo de prueba
    int result = Compressor_lz77(FILENAME, FILENAME_COMPR, 2010+1200, 1200);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_1000B1000 (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }

    unsigned char c;

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_COMPR, codesize, 0);
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (c != code[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_1000B1000 (DecompressorTest) message=i=%d) leido:%d != deberiaSer:%d\n", i, c, code[i]);
        } else {
            //printf("%d) %d == %d\n", i, c, decode[i]);
        }
    }
    if (i != codesize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLz77_1000B1000 (DecompressorTest) message=Se leyeron %d caracteres en lugar de %d.\n", i, codesize);
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testCompressor_compressLzhuff() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    const unsigned char* FILECONTENT = "jerryrice";
    int filecontentsize = 9;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), filecontentsize, fp);
    fclose(fp);

    // comprimo el archivo de prueba
    int result = Compressor_lzhuff(FILENAME, FILENAME_COMPR, filecontentsize+1, filecontentsize);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }

    // Descomprimo el archivo de prueba
    if(Decompressor_lzhuff(FILENAME_COMPR, FILENAME, 3, filecontentsize) != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff (CompressorTest) message=Error al descomprimir\n");
        return;
    }

    // Leo el archivo descomprimido
    unsigned char buffer[filecontentsize];
    fp = fopen(FILENAME, "rb");
    int length = fread(buffer, sizeof(unsigned char), filecontentsize, fp);
    fclose(fp);

    if(length != filecontentsize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff (CompressorTest) message=No coinciden las longitudes: %d != %d\n", length, filecontentsize);
        return;
    }
    // Comparo el archivo descomprimido con el original
    int i;
    for (i=0; i<length; i++) {
        if(FILECONTENT[i] != buffer[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff (CompressorTest) message=%d) El caracter %d del archivo descomprimido no coincide con el original %d\n", i, buffer[i], FILECONTENT[i]);
            return;
        }
    }
}


void testCompressor_compressLzhuff_AABBAB() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    const unsigned char* FILECONTENT = "AABBAB";
    int filecontentsize = 6;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), filecontentsize, fp);
    fclose(fp);

    // comprimo el archivo de prueba
    int result = Compressor_lzhuff(FILENAME, FILENAME_COMPR, filecontentsize+1, filecontentsize);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_AABBAB (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }

    // Descomprimo el archivo de prueba
    if(Decompressor_lzhuff(FILENAME_COMPR, FILENAME, 3, filecontentsize) != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_AABBAB (CompressorTest) message=Error al descomprimir\n");
        return;
    }

    // Leo el archivo descomprimido
    unsigned char buffer[filecontentsize];
    fp = fopen(FILENAME, "rb");
    int length = fread(buffer, sizeof(unsigned char), filecontentsize, fp);
    fclose(fp);

    if(length != filecontentsize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_AABBAB (CompressorTest) message=No coinciden las longitudes: %d != %d\n", length, filecontentsize);
        return;
    }
    // Comparo el archivo descomprimido con el original
    int i;
    for (i=0; i<length; i++) {
        if(FILECONTENT[i] != buffer[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_AABBAB (CompressorTest) message=%d) El caracter %d del archivo descomprimido no coincide con el original %d\n", i, buffer[i], FILECONTENT[i]);
            return;
        }
    }
}


void testCompressor_compressLzhuff_AAAAA() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    const unsigned char* FILECONTENT = "AAAAA";
    int filecontentsize = 5;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), filecontentsize, fp);
    fclose(fp);

    // comprimo el archivo de prueba
    int result = Compressor_lzhuff(FILENAME, FILENAME_COMPR, filecontentsize+1, filecontentsize);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_AAAAA (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }

    // Descomprimo el archivo de prueba
    if(Decompressor_lzhuff(FILENAME_COMPR, FILENAME, 3, filecontentsize) != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_AAAAA (CompressorTest) message=Error al descomprimir\n");
        return;
    }

    // Leo el archivo descomprimido
    unsigned char buffer[filecontentsize];
    fp = fopen(FILENAME, "rb");
    int length = fread(buffer, sizeof(unsigned char), filecontentsize, fp);
    fclose(fp);

    if(length != filecontentsize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_AAAAA (CompressorTest) message=No coinciden las longitudes: %d != %d\n", length, filecontentsize);
        return;
    }
    // Comparo el archivo descomprimido con el original
    int i;
    for (i=0; i<length; i++) {
        if(FILECONTENT[i] != buffer[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_AAAAA (CompressorTest) message=%d) El caracter %d del archivo descomprimido no coincide con el original %d\n", i, buffer[i], FILECONTENT[i]);
            return;
        }
    }
}


void testCompressor_compressLzhuff_AAAABBBB() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    const unsigned char* FILECONTENT = "AAAABBBB";
    int filecontentsize = 8;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), filecontentsize, fp);
    fclose(fp);

    // comprimo el archivo de prueba
    int result = Compressor_lzhuff(FILENAME, FILENAME_COMPR, filecontentsize+1, filecontentsize);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_AAAABBBB (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }

    // Descomprimo el archivo de prueba
    if(Decompressor_lzhuff(FILENAME_COMPR, FILENAME, 3, filecontentsize) != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_AAAABBBB (CompressorTest) message=Error al descomprimir\n");
        return;
    }

    // Leo el archivo descomprimido
    unsigned char buffer[filecontentsize];
    fp = fopen(FILENAME, "rb");
    int length = fread(buffer, sizeof(unsigned char), filecontentsize, fp);
    fclose(fp);

    if(length != filecontentsize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_AAAABBBB (CompressorTest) message=No coinciden las longitudes: %d != %d\n", length, filecontentsize);
        return;
    }
    // Comparo el archivo descomprimido con el original
    int i;
    for (i=0; i<length; i++) {
        if(FILECONTENT[i] != buffer[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_AAAABBBB (CompressorTest) message=%d) El caracter %d del archivo descomprimido no coincide con el original %d\n", i, buffer[i], FILECONTENT[i]);
            return;
        }
    }
}


void testCompressor_compressLzhuff_ABCDABCD() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    const unsigned char* FILECONTENT = "ABCDABCD";
    int filecontentsize = 8;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), filecontentsize, fp);
    fclose(fp);

    // comprimo el archivo de prueba
    int result = Compressor_lzhuff(FILENAME, FILENAME_COMPR, filecontentsize+1, filecontentsize);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_ABCDABCD (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }

    // Descomprimo el archivo de prueba
    if(Decompressor_lzhuff(FILENAME_COMPR, FILENAME, 3, filecontentsize) != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_ABCDABCD (CompressorTest) message=Error al descomprimir\n");
        return;
    }

    // Leo el archivo descomprimido
    unsigned char buffer[filecontentsize];
    fp = fopen(FILENAME, "rb");
    int length = fread(buffer, sizeof(unsigned char), filecontentsize, fp);
    fclose(fp);

    if(length != filecontentsize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_ABCDABCD (CompressorTest) message=No coinciden las longitudes: %d != %d\n", length, filecontentsize);
        return;
    }
    // Comparo el archivo descomprimido con el original
    int i;
    for (i=0; i<length; i++) {
        if(FILECONTENT[i] != buffer[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_ABCDABCD (CompressorTest) message=%d) El caracter %d del archivo descomprimido no coincide con el original %d\n", i, buffer[i], FILECONTENT[i]);
            return;
        }
    }
}


void testCompressor_compressLzhuff_ABAAB() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    const unsigned char* FILECONTENT = "ABAAB";
    int filecontentsize = 5;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), filecontentsize, fp);
    fclose(fp);

    // comprimo el archivo de prueba
    int result = Compressor_lzhuff(FILENAME, FILENAME_COMPR, filecontentsize+1, filecontentsize);
    if(result != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_ABAAB (CompressorTest) message=Error al comprimir: %d\n", result);
        return;
    }

    // Descomprimo el archivo de prueba
    if(Decompressor_lzhuff(FILENAME_COMPR, FILENAME, 3, filecontentsize) != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_ABAAB (CompressorTest) message=Error al descomprimir\n");
        return;
    }

    // Leo el archivo descomprimido
    unsigned char buffer[filecontentsize];
    fp = fopen(FILENAME, "rb");
    int length = fread(buffer, sizeof(unsigned char), filecontentsize, fp);
    fclose(fp);

    if(length != filecontentsize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_ABAAB (CompressorTest) message=No coinciden las longitudes: %d != %d\n", length, filecontentsize);
        return;
    }
    // Comparo el archivo descomprimido con el original
    int i;
    for (i=0; i<length; i++) {
        if(FILECONTENT[i] != buffer[i]) {
            printf("%%TEST_FAILED%% time=0 testname=testCompressor_compressLzhuff_ABAAB (CompressorTest) message=%d) El caracter %d del archivo descomprimido no coincide con el original %d\n", i, buffer[i], FILECONTENT[i]);
            return;
        }
    }
}


void testCompressor_moveToFront() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {'a', 'b', 'r', 'a', 'c', 'a', 'd', 'a', 'b'};
    unsigned char code[] = {'a', 'b', 'r', 2, 'd', 1, 'e', 1, 4};
    size_t n = 9;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), n, fp);
    fclose(fp);

    Compressor_moveToFront(FILENAME, FILENAME_COMPR, 4);

    BlockBufferedReader reader;
    BlockBufferedReader_open(&reader, FILENAME_COMPR, n, 0);

    int i;
    for (i=0; i<n; i++) {
        if (code[i] != reader.buffer[i]) break;
    }
    if(i != n) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_moveToFront (CompressorTest) message=mtf[%d]=%d buffer[%d]=%d\n", i, code[i], i, reader.buffer[i]);
        return;
    }

    BlockBufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testCompressor_blockSorting() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {5, 0, 3, 0, 1, 0, 2, 0, 4, 6, 0};
    size_t index = 9;
    unsigned char code[] = {3, 1, 5, 2, 6, 0, 0, 0, 0, 0, 4};
    size_t n = 11;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), n, fp);
    fclose(fp);

    Compressor_blockSorting(FILENAME, FILENAME_COMPR, n);

    BlockBufferedReader reader;
    BlockBufferedReader_open(&reader, FILENAME_COMPR, n + sizeof(size_t), 0);

    size_t ii = 0;
    if (!BlockBufferedReader_readNextSize_t(&reader, &ii)) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSorting (CompressorTest) message=dio eof\n");
        return;
    }

    if (index != ii) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSorting (CompressorTest) message=el indice no coincide: %d != %d\n", (int)ii, (int)index);
        return;
    }

    int i;
    for (i=0; i<n; i++) {
        if (code[i] != reader.buffer[i + reader.windowSize + sizeof(size_t)]) break;
    }
    if(i != n) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSorting (CompressorTest) message=mtf[%d]=%d buffer[%d]=%d\n", i, code[i], i, reader.buffer[i]);
        return;
    }

    BlockBufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testCompressor_blockSortingN4() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {5, 0, 3, 0, 1, 0, 2, 0, 4, 6, 0};
    size_t len = 11;
    size_t index = 9;
    unsigned char code[] = {5, 3, 0, 0, 2, 1, 0, 0, 6, 0, 4};
    size_t n = 4;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), len, fp);
    fclose(fp);

    Compressor_blockSorting(FILENAME, FILENAME_COMPR, n);

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_COMPR, n*sizeof(size_t) + len, 0);

    int i;
    size_t ii = 0;
    unsigned char c;
    index = 3;
    if (!BufferedReader_readNextSize_t(&reader, &ii)) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSortingN4 (CompressorTest) message=dio eof\n");
        return;
    }
    if (index != ii) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSortingN4 (CompressorTest) message=el indice no coincide: %d != %d\n", (int)ii, (int)index);
        return;
    }
    for (i=0; i<n && BufferedReader_readNextByte(&reader, &c); i++) {
        if (code[i] != c) break;
    }
    if(i != n) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSortingN4 (CompressorTest) message=mtf[%d]=%d buffer[%d]=%d\n", i, code[i], i, c);
        return;
    }

    index = 2;
    if (!BlockBufferedReader_readNextSize_t(&reader, &ii)) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSortingN4 (CompressorTest) message=dio eof\n");
        return;
    }
    if (index != ii) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSortingN4 (CompressorTest) message=el indice no coincide: %d != %d\n", (int)ii, (int)index);
        return;
    }
    for (i=n; i<2*n && BufferedReader_readNextByte(&reader, &c); i++) {
        if (code[i] != c) break;
    }
    if(i != 2*n) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSortingN4 (CompressorTest) message=mtf[%d]=%d buffer[%d]=%d\n", i, code[i], i, c);
        return;
    }

    index = 1;
    if (!BlockBufferedReader_readNextSize_t(&reader, &ii)) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSortingN4 (CompressorTest) message=dio eof\n");
        return;
    }
    if (index != ii) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSortingN4 (CompressorTest) message=el indice no coincide: %d != %d\n", (int)ii, (int)index);
        return;
    }
    for (i=2*n; i<len && BufferedReader_readNextByte(&reader, &c); i++) {
        if (code[i] != c) break;
    }
    if(i != len) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSortingN4 (CompressorTest) message=mtf[%d]=%d buffer[%d]=%d\n", i, code[i], i, c);
        return;
    }

    BlockBufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testCompressor_blockSortingMoveToFront() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {5, 0, 3, 0, 1, 0, 2, 0, 4, 6, 0};
    size_t index = 9;
    unsigned char code[] = {3, 2, 5, 4, 6, 5, 0, 0, 0, 0, 6};
    size_t n = 11;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), n, fp);
    fclose(fp);

    Compressor_blockSortingMoveToFront(FILENAME, FILENAME_COMPR, n);

    BlockBufferedReader reader;
    BlockBufferedReader_open(&reader, FILENAME_COMPR, n + sizeof(size_t), 0);

    size_t ii = 0;
    if (!BlockBufferedReader_readNextSize_t(&reader, &ii)) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSortingMoveToFront (CompressorTest) message=dio eof\n");
        return;
    }

    if (index != ii) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSortingMoveToFront (CompressorTest) message=el indice no coincide: %d != %d\n", (int)ii, (int)index);
        return;
    }

    int i;
    for (i=0; i<n; i++) {
        if (code[i] != reader.buffer[i + reader.windowSize + sizeof(size_t)]) break;
    }
    if(i != n) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSortingMoveToFront (CompressorTest) message=mtf[%d]=%d buffer[%d]=%d\n", i, code[i], i, reader.buffer[i]);
        return;
    }

    BlockBufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testCompressor_blockSortingMoveToFrontBlockSorting() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {5, 0, 3, 0, 1, 0, 2, 0, 4, 6, 0};
    unsigned char code[] = {5, 0, 0, 0, 3, 6, 5, 6, 2, 0, 4};
    size_t n = 11;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), n, fp);
    fclose(fp);

    Compressor_blockSortingMoveToFrontBlockSorting(FILENAME, FILENAME_COMPR, n);

    BlockBufferedReader reader;
    BlockBufferedReader_open(&reader, FILENAME_COMPR, n, 0);

    int i;
    for (i=0; i<n; i++) {
        if (code[i] != reader.buffer[i]) break;
    }
    if(i != n) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_blockSortingMoveToFrontBlockSorting (CompressorTest) message=mtf[%d]=%d buffer[%d]=%d\n", i, code[i], i, reader.buffer[i]);
        return;
    }

    BlockBufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


void testCompressor_lzss() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    const char* FILENAME_COMPR = "pepe.txt.XX";
    unsigned char FILECONTENT[] = {1, 1, 2, 2, 1, 2};
    //unsigned char code[] = {0, 192, 64, 0, 80, 16, 8, 16, 20};
    unsigned char code[] = {0, 128, 64, 64, 40, 16, 20};
    //int codesize = 9;
    int codesize = 7;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(char), 6, fp);
    fclose(fp);

    Compressor_lzss(FILENAME, FILENAME_COMPR, 4, 3);

    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME_COMPR, 4, 0);

    unsigned char c;
    int i;
    for (i=0; BufferedReader_readNextByte(&reader, &c); i++) {
        if (code[i] != c) break;
    }
    if(i != codesize) {
        printf("%%TEST_FAILED%% time=0 testname=testCompressor_lzss (CompressorTest) message=lzss[%d]=%d buffer[%d]=%d\n", i, code[i], i, reader.buffer[i]);
        return;
    }

    BufferedReader_close(&reader);
    remove(FILENAME);
    remove(FILENAME_COMPR);
}


int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% CompressorTest\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%%  testCompressor_compressDynamicHuffman (CompressorTest)\n");
    testCompressor_compressDynamicHuffman();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressDynamicHuffman (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_moveToFront (CompressorTest)\n");
    testCompressor_moveToFront();
    printf("%%TEST_FINISHED%% time=0 testCompressor_moveToFront (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_blockSorting (CompressorTest)\n");
    testCompressor_blockSorting();
    printf("%%TEST_FINISHED%% time=0 testCompressor_blockSorting (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_blockSortingN4 (CompressorTest)\n");
    testCompressor_blockSortingN4();
    printf("%%TEST_FINISHED%% time=0 testCompressor_blockSortingN4 (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_blockSortingMoveToFront (CompressorTest)\n");
    testCompressor_blockSortingMoveToFront();
    printf("%%TEST_FINISHED%% time=0 testCompressor_blockSortingMoveToFront (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_blockSortingMoveToFrontBlockSorting (CompressorTest)\n");
    testCompressor_blockSortingMoveToFrontBlockSorting();
    printf("%%TEST_FINISHED%% time=0 testCompressor_blockSortingMoveToFrontBlockSorting (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_compressLz77_AAAAA (CompressorTest)\n");
    testCompressor_compressLz77_AAAAA();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressLz77_AAAAA (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_compressLz77_500 (CompressorTest)\n");
    testCompressor_compressLz77_500();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressLz77_500 (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_compressLz77_1000 (CompressorTest)\n");
    testCompressor_compressLz77_1000();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressLz77_1000 (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_compressLz77_766 (CompressorTest)\n");
    testCompressor_compressLz77_766();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressLz77_766 (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_compressLz77_767 (CompressorTest)\n");
    testCompressor_compressLz77_767();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressLz77_767 (CompressorTest)\n");
    
    printf("%%TEST_STARTED%%  testCompressor_compressLz77_1000B1000 (CompressorTest)\n");
    testCompressor_compressLz77_1000B1000();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressLz77_1000B1000 (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_compressLz77_766B766 (CompressorTest)\n");
    testCompressor_compressLz77_766B766();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressLz77_766B766 (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_lzss (CompressorTest)\n");
    testCompressor_lzss();
    printf("%%TEST_FINISHED%% time=0 testCompressor_lzss (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_compressLzhuff (CompressorTest)\n");
    testCompressor_compressLzhuff();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressLzhuff (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_compressLzhuff_AABBAB (CompressorTest)\n");
    testCompressor_compressLzhuff_AABBAB();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressLzhuff_AABBAB (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_compressLzhuff_AAAAA (CompressorTest)\n");
    testCompressor_compressLzhuff_AAAAA();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressLzhuff_AAAAA (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_compressLzhuff_AAAABBBB (CompressorTest)\n");
    testCompressor_compressLzhuff_AAAABBBB();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressLzhuff_AAAABBBB (CompressorTest)\n");

    printf("%%TEST_STARTED%%  testCompressor_compressLzhuff_ABCDABCD (CompressorTest)\n");
    testCompressor_compressLzhuff_ABCDABCD();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressLzhuff_ABCDABCD (CompressorTest)\n");
        
    printf("%%TEST_STARTED%%  testCompressor_compressLzhuff_ABAAB (CompressorTest)\n");
    testCompressor_compressLzhuff_ABAAB();
    printf("%%TEST_FINISHED%% time=0 testCompressor_compressLzhuff_ABAAB (CompressorTest)\n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
