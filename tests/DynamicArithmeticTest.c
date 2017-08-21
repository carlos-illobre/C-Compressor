/*
 * File:   DynamicArithmeticTest.c
 * Author: Carlos Illobre
 *
 * Created on 02/04/2011, 05:55:04
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "DynamicArithmetic.h"
#include "BufferedReader.h"


void testDynamicArithmetic_create() {
    unsigned int i = 256;
    DynamicArithmetic this;
    DynamicArithmetic_create(&this, i);

    // Debe tener tamaño 256
    if(this.size != i) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_create (DynamicArithmetic) message=El size deberia ser %d pero es %d\n", (int)i, (int)this.size);
        return;
    }

    // Count debe ser igual a size
    if(this.size != this.count) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_create (DynamicArithmetic) message=El size es %d y count es %d\n", (int)this.count, (int)this.size);
        return;
    }

    // debe contener todos los simbolos con frecuencia 1
    for (i=0; i < this.size; i++) {
        if(this.freq[i] != 1) {
            printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_create (DynamicArithmetic) message=El simbolo %lu no tiene frecuenca 1\n", i);
            return;
        }
    }

    // El techo debe ser el numero maximo posible
    if (this.ceil != CEILFLOORTYPE_MAX) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_create (DynamicArithmetic) message=El valor maximo posible es %ull y el techo contiene el valor %ull\n", CEILFLOORTYPE_MAX, this.ceil);
        return;
    }

    // El piso no es cero
    if (this.floor != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_create (DynamicArithmetic) message=El piso no es cero: %ul\n", this.floor);
        return;
    }

    // Underflow debe ser cero
/*
    if (this.underflow) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_create (DynamicArithmetic) message=El piso no es cero: %ul\n", this.floor);
        return;
    }
*/

    // No se esta descomprimiendo
    if (this.isDecompress) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_create (DynamicArithmetic) message=Flag de descompresion activado\n");
        return;
    }

    DynamicArithmetic_destroy(&this);
}


void testDynamicArithmetic_incFreq() {
    size_t i = 256;
    DynamicArithmetic this;
    DynamicArithmetic_create(&this, i);

    unsigned int symbol = 64;
    unsigned int freq = this.freq[symbol];
    unsigned int count = this.count;
    DynamicArithmetic_incFreq(&this, symbol);

    // Al incrementar el caracter su frecuencia debe ser la anterior mas 1
    if(freq+1 != this.freq[symbol]) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_incFreq (DynamicArithmetic) message=La frecuencia deberia valer %u pero vale %u\n", freq+1, this.freq[symbol]);
        return;
    }

    // Count debe ser el count anterior mas 1
    if(count+1 != this.count) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_incFreq (DynamicArithmetic) message=Count deberia valer %u pero vale %u\n", count+1, this.count);
        return;
    }

    unsigned int anterior;
    // Si la minima frecuencia es 1 y count llego a su maximo, al incrementar la
    // minima debe seguir siendo 1, count debe valer la mitad y las frecuencias
    // distintas de 1 deben valer la mitad
    this.freq[40] += UINT_MAX - this.count -1;
    freq = this.freq[40];
    this.count = UINT_MAX -1;
    if(this.freq[0] != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_incFreq (DynamicArithmetic) message=La frecuencia deberia valer %u pero vale %u\n", 1, this.freq[0]);
        return;
    }
    if(this.count != UINT_MAX-1) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_incFreq (DynamicArithmetic) message=Count deberia valer %u pero vale %u\n", UINT_MAX-1, this.count);
        return;
    }
    DynamicArithmetic_incFreq(&this, symbol);
    if(this.freq[0] != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_incFreq (DynamicArithmetic) message=La frecuencia deberia valer %u pero vale %u\n", 1, this.freq[0]);
        return;
    }
    if(this.freq[40] != freq/2) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_incFreq (DynamicArithmetic) message=La frecuencia deberia valer %u pero vale %u\n", freq/2, this.freq[40]);
        return;
    }
    if(this.count != UINT_MAX/2) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_incFreq (DynamicArithmetic) message=Count deberia valer %u pero vale %u\n", UINT_MAX/2, this.count);
        return;
    }

    DynamicArithmetic_destroy(&this);
}


void testDynamicArithmetic_compressDecompressOneSymbol() {
    // Creo un archivo
    const char* FILENAME = "pepe.txt";

    // Construyo un compresor
    size_t size = 256;
    DynamicArithmetic this;
    unsigned int symbol;

    for (symbol=0; symbol<size; symbol++) {
        BufferedWriter* writer = BufferedWriter_open(FILENAME, 10);
        DynamicArithmetic_create(&this, size);
        unsigned int result = 1;
        // Escribo un caracter en el archivo
        DynamicArithmetic_compress(&this, symbol, writer);
        DynamicArithmetic_compressEof(&this, writer);
        BufferedWriter_closeWithEof(writer);

        DynamicArithmetic_destroy(&this);
        DynamicArithmetic_create(&this, size);

        // Leo el caracter
        BufferedReader reader;
        BufferedReader_open(&reader, FILENAME, 10, 0);
        if (!DynamicArithmetic_decompress(&this, &result, &reader)) {
            printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_compressDecompressOneSymbol (DynamicHuffmanTest) message=Se esta indicando que no se pudo leer el caracter.\n");
            return;
        }
        if (DynamicArithmetic_decompress(&this, &result, &reader)) {
            printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_compressDecompressOneSymbol (DynamicHuffmanTest) message=No se indico que no hay mas caracteres.\n");
            return;
        }

        // Los caracteres deben coincidir
        if (symbol != result) {
            printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_compressDecompressOneSymbol (DynamicHuffmanTest) message=Error en la lectura, se leyo %u y se esperaba %u.\n", result, symbol);
            return;
        }
        DynamicArithmetic_destroy(&this);
        BufferedReader_close(&reader);
    }

    
}


void testDynamicArithmetic_compressDecompressTwoSymbols() {
    // Creo un archivo
    const char* FILENAME = "pepe.txt";

    // Construyo un compresor
    size_t size = 256;
    DynamicArithmetic this;
    unsigned int symbol1 = 4;
    unsigned int symbol2 = 37;

    BufferedWriter* writer = BufferedWriter_open(FILENAME, 10);
    DynamicArithmetic_create(&this, size);
    unsigned int result = 1;
    // Escribo los caracteres en el archivo
    DynamicArithmetic_compress(&this, symbol1, writer);
    DynamicArithmetic_compress(&this, symbol2, writer);
    DynamicArithmetic_compressEof(&this, writer);
    BufferedWriter_closeWithEof(writer);

    DynamicArithmetic_destroy(&this);
    DynamicArithmetic_create(&this, size);

    // Leo el caracter
    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, 10, 0);

    DynamicArithmetic_decompress(&this, &result, &reader);
    // Los caracteres deben coincidir
    if (symbol1 != result) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_compressDecompressOneSymbol (DynamicHuffmanTest) message=Error en la lectura, se leyo %u y se esperaba %u.\n", result, symbol1);
        return;
    }

    DynamicArithmetic_decompress(&this, &result, &reader);
    // Los caracteres deben coincidir
    if (symbol2 != result) {
        printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_compressDecompressOneSymbol (DynamicHuffmanTest) message=Error en la lectura, se leyo %u y se esperaba %u.\n", result, symbol2);
        return;
    }


    DynamicArithmetic_destroy(&this);
    BufferedReader_close(&reader);
}


void testDynamicArithmetic_compressDecompress() {
    size_t size = 256;
    DynamicArithmetic this;
    DynamicArithmetic_create(&this, size);

    // Creo un archivo
    const char* FILENAME = "pepe.txt";
    BufferedWriter* writer = BufferedWriter_open(FILENAME, 10);

    int i;
    // Incremento un caracter
    for (i=0; i<10; i++) DynamicArithmetic_incFreq(&this, 0);

    // Incremento un caracter
    for (i=0; i<100; i++) DynamicArithmetic_incFreq(&this, 56);

    // Incremento un caracter
    for (i=0; i<15; i++) DynamicArithmetic_incFreq(&this, 200);

    // Incremento un caracter
    for (i=0; i<4; i++) DynamicArithmetic_incFreq(&this, 128);

    // Incremento un caracter
    for (i=0; i<99; i++) DynamicArithmetic_incFreq(&this, 255);

    unsigned int symbol;
    unsigned int result = 1;

    // Escribo todos los caracteres en el archivo
    for (symbol=0; symbol<size; symbol++) {
        DynamicArithmetic_compress(&this, symbol, writer);
    }
    DynamicArithmetic_compressEof(&this, writer);
    BufferedWriter_closeWithEof(writer);
    DynamicArithmetic_destroy(&this);

    DynamicArithmetic_create(&this, size);
    // Incremento un caracter
    for (i=0; i<10; i++) DynamicArithmetic_incFreq(&this, 0);

    // Incremento un caracter
    for (i=0; i<100; i++) DynamicArithmetic_incFreq(&this, 56);

    // Incremento un caracter
    for (i=0; i<15; i++) DynamicArithmetic_incFreq(&this, 200);

    // Incremento un caracter
    for (i=0; i<4; i++) DynamicArithmetic_incFreq(&this, 128);

    // Incremento un caracter
    for (i=0; i<99; i++) DynamicArithmetic_incFreq(&this, 255);

    // Leo los caracteres
    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, 10, 0);
    for (symbol=0; DynamicArithmetic_decompress(&this, &result, &reader); symbol++) {
        // Los caracteres deben coincidir
        if (symbol != result) {
            printf("%%TEST_FAILED%% time=0 testname=testDynamicArithmetic_compressDecompress (DynamicHuffmanTest) message=Error en la lectura, se leyo %d y se esperaba %d.\n", result, symbol);
            return;
        }
    }

    BufferedReader_close(&reader);

    DynamicArithmetic_destroy(&this);
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% DynamicArithmetic\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%%  testDynamicArithmetic_create (DynamicArithmetic)\n");
    testDynamicArithmetic_create();
    printf("%%TEST_FINISHED%% time=0 testDynamicArithmetic_create (DynamicArithmetic)\n");

    printf("%%TEST_STARTED%%  testDynamicArithmetic_incFreq (DynamicArithmetic)\n");
    testDynamicArithmetic_incFreq();
    printf("%%TEST_FINISHED%% time=0 testDynamicArithmetic_incFreq (DynamicArithmetic)\n");

    printf("%%TEST_STARTED%%  testDynamicArithmetic_compressDecompressOneSymbol (DynamicArithmetic)\n");
    testDynamicArithmetic_compressDecompressOneSymbol();
    printf("%%TEST_FINISHED%% time=0 testDynamicArithmetic_compressDecompressOneSymbol (DynamicArithmetic)\n");

    printf("%%TEST_STARTED%%  testDynamicArithmetic_compressDecompress (DynamicArithmetic)\n");
    testDynamicArithmetic_compressDecompress();
    printf("%%TEST_FINISHED%% time=0 testDynamicArithmetic_compressDecompress (DynamicArithmetic)\n");

    printf("%%TEST_STARTED%%  testDynamicArithmetic_compressDecompressTwoSymbols (DynamicArithmetic)\n");
    testDynamicArithmetic_compressDecompressTwoSymbols();
    printf("%%TEST_FINISHED%% time=0 testDynamicArithmetic_compressDecompressTwoSymbols (DynamicArithmetic)\n");
    
    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
