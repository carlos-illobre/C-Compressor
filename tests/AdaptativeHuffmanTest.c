/*
 * File:   AdaptativeHuffmanTest.c
 * Author: Carlos Illobre
 *
 * Created on 21/03/2011, 23:16:21
 */

#include <stdio.h>
#include <stdlib.h>
#include "AdaptativeHuffman.h"
#include "BufferedBitReader.h"


int testFreq(const AdaptativeHuffmanNotLeaf* parent) {
    // Resultado del test
    int testPassed;
    
    // Obtengo las frecuencias
    size_t freqParent = AdaptativeHuffmanNode_getFreq((AdaptativeHuffmanNode*)parent);
    size_t freqLeft = AdaptativeHuffmanNode_getFreq(parent->leftChild);
    size_t freqRight = AdaptativeHuffmanNode_getFreq(parent->rightChild);
    
    // El test se pasa si la frecuencia del padre es igual a la suma de las frecuencias de los hijos
    testPassed = freqParent == freqLeft + freqRight;
    
    // Si el test se viene pasando y el hijo izquierdo no es hoja
    if (testPassed && !AdaptativeHuffmanNode_isLeaf(parent->leftChild)) {
        // Me fijo si el hijo izquierdo pasa el test
        testPassed = testFreq((AdaptativeHuffmanNotLeaf*)parent->leftChild);
    }
    
    // Si el test se viene pasando y el hijo derecho no es hoja
    if (testPassed && !AdaptativeHuffmanNode_isLeaf(parent->rightChild)) {
        // Me fijo si el hijo derecho pasa el test
        testPassed = testFreq((AdaptativeHuffmanNotLeaf*)parent->rightChild);
    }
    
    // Retorno el resultado del test
    return testPassed;
}


void testAdaptativeHuffman_init() {
    AdaptativeHuffman this;
    AdaptativeHuffman_init(&this);
    // La raiz debe tener frecuencia igual a la cantidad de hojas
    size_t freq = AdaptativeHuffmanNode_getFreq((AdaptativeHuffmanNode*)this.root);
    if (freq != HUFFMAN_LEAF_COUNT) {
        printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_init (AdaptativeHuffmanTest) message=Error en frecuencia de raiz: '%d'\n", freq);
        return;
    }

    // La frecuencia de los padres es igual a la suma de las frecuencias de los hijos
    if (!testFreq(this.root)) {
        printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_init (AdaptativeHuffmanTest) message=Las frecuencias no coinciden.\n");
    }

    // El arbol debe contener todos los caracteres con frecuencia 1
    ArrayBit codes[HUFFMAN_LEAF_COUNT];
    int c;
    for (c=0; c<HUFFMAN_LEAF_COUNT; c++) {
        ArrayBit_create(codes+c, 32);
    }
    for (c=0; c<HUFFMAN_LEAF_COUNT; c++) {
        // Debe contener todos los caracteres
        freq = AdaptativeHuffman_getCode_ArrayBit(&this, c, codes + c);
        if (freq != 1) {
            printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_init (AdaptativeHuffmanTest) message=Falta el caracter: '%d' o su frecuencia es %d.\n", c, freq);
            return;
        }
        // los codigos inicialente tienen 9 bits
        if (codes[c].length != 9) {
            printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_init (AdaptativeHuffmanTest) message=El codigo del caracter '%d' deberia tener 9 bits pero tiene %d.\n", c, codes[c].length);
        }
    }

    // Todos los caracteres deben tener codigos diferentes
    int j;
    for (c=0; c<HUFFMAN_LEAF_COUNT; c++) {
        for (j=0; j<HUFFMAN_LEAF_COUNT; j++) {
            if (c != j && ArrayBit_equals(codes+c, codes+j)) {
                printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_init (AdaptativeHuffmanTest) message=El caracter '%d' tiene el mismo codigo que el caracter %d.\n", c, j);
                c = j = HUFFMAN_LEAF_COUNT;
            }
        }
    }

    for (c=0; c<HUFFMAN_LEAF_COUNT; c++) {
        ArrayBit_destroy(codes+c);
    }
}


int testAdaptativeHuffman_getCharacter(const AdaptativeHuffman* this) {
    ArrayBit code;
    ArrayBit_create(&code, 32);
    Symbol symbol;
    int i;
    for (i=0; i<HUFFMAN_LEAF_COUNT; i++) {
        if (!AdaptativeHuffman_getCode_ArrayBit(this, i, &code)) {
            printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_incFreq (AdaptativeHuffmanTest) message=No se encuentra el caracter %d\n", i);
            ArrayBit_destroy(&code);
            return 0;
        }
        symbol = AdaptativeHuffman_getSymbol_ArrayBit(this, &code);
        if (symbol != i) {
            printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_incFreq (AdaptativeHuffmanTest) message=No coincide el codigo del caracter %d\n", i);
            ArrayBit_destroy(&code);
            return 0;
        }
    }
    ArrayBit_destroy(&code);
    return 1;
}


void testAdaptativeHuffman_getCharacterTwoInstances() {
    AdaptativeHuffman this;
    AdaptativeHuffman_init(&this);
    AdaptativeHuffman other;
    AdaptativeHuffman_init(&other);

    ArrayBit code;
    ArrayBit_create(&code, 32);
    Symbol symbol;
    int i;
    for (i=0; i<HUFFMAN_LEAF_COUNT; i++) {
        if (!AdaptativeHuffman_getCode_ArrayBit(&this, i, &code)) {
            printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_getCharacterTwoInstances (AdaptativeHuffmanTest) message=No se encuentra el caracter %d\n", i);
            ArrayBit_destroy(&code);
            return 0;
        }
        symbol = AdaptativeHuffman_getSymbol_ArrayBit(&other, &code);
        if (symbol != i) {
            printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_getCharacterTwoInstances (AdaptativeHuffmanTest) message=No coincide el codigo del caracter %d\n", i);
            ArrayBit_destroy(&code);
            return 0;
        }
    }
    ArrayBit_destroy(&code);
    return 1;
}


void testAdaptativeHuffman_incFreq() {
    AdaptativeHuffman this;
    AdaptativeHuffman_init(&this);
    // Inicialmente todos los getCharacter se corresponden con los getCode
    if (!testAdaptativeHuffman_getCharacter(&this)) {
        return;
    }
    
    unsigned char c=0;
    ArrayBit result1, result2;
    ArrayBit_create(&result1, 32);
    ArrayBit_create(&result2, 32);
    // Obtengo el codigo de un caracter antes de incrementar
    AdaptativeHuffman_getCode_ArrayBit(&this, c, &result1);
    // incremento 2 veces
    AdaptativeHuffman_incFreq(&this, c);
    if (!testAdaptativeHuffman_getCharacter(&this)) {
        return;
    }
    // La frecuencia de los padres es igual a la suma de las frecuencias de los hijos
    if (!testFreq(this.root)) {
        printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_incFreq (AdaptativeHuffmanTest) message=Las frecuencias no coinciden.\n");
        return;
    }

    AdaptativeHuffman_incFreq(&this, c);
    if (!testAdaptativeHuffman_getCharacter(&this)) {
        return;
    }
    // La frecuencia de los padres es igual a la suma de las frecuencias de los hijos
    if (!testFreq(this.root)) {
        printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_incFreq (AdaptativeHuffmanTest) message=Las frecuencias no coinciden.\n");
        return;
    }

    // Obtengo el codigo y la frecuencia luego de incrementar dos veces
    size_t freq = AdaptativeHuffman_getCode_ArrayBit(&this, c, &result2);
    if (freq != 3) {
        printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_incFreq (AdaptativeHuffmanTest) message=La frecuencia del caracter no se incremento, deberia ser 3 y es %d.\n", freq);
        return;
    }
    // Si el codigo luego de incrementar no es menor que al principio hay error
    if (result2.length >= result1.length) {
        printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_incFreq (AdaptativeHuffmanTest) message=La longitud del codigo no disminuyo.\n");
        return;
    }
    
    ArrayBit_destroy(&result1);
    ArrayBit_destroy(&result2);
}


void testAdaptativeHuffman_BufferedBitReader() {
    // Creo un archivo
    const char* FILENAME = "pepe.txt";
    BufferedWriter* writer = BufferedWriter_open(FILENAME, 10);

    // Construyo un arbol
    AdaptativeHuffman this;
    AdaptativeHuffman_init(&this);

    Symbol symbol = 0;
    Symbol result = 1;
    // Escribo un caracter en el archivo
    AdaptativeHuffman_getCode_BufferedWriter(&this, symbol, writer);
    BufferedWriter_closeWithEof(writer);

    // Leo el caracter
    BufferedBitReader reader;
    BufferedBitReader_open(&reader, FILENAME, 10);
    AdaptativeHuffman_getSymbol_BufferedBitReader(&this, &result, &reader);

    // Los caracteres deben coincidir
    if (symbol != result) {
        printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_BufferedBitReader (AdaptativeHuffmanTest) message=Error en la lectura, se leyo %d y se esperaba %d.\n", result, symbol);
    }

    BufferedBitReader_close(&reader);
}


void testAdaptativeHuffman_BufferedBitReaderAllChar() {
    // Creo un archivo
    const char* FILENAME = "pepe.txt";
    BufferedWriter* writer = BufferedWriter_open(FILENAME, 10);

    // Construyo un arbol
    AdaptativeHuffman this;
    AdaptativeHuffman_init(&this);

    int i;
    // Incremento un caracter
    for (i=0; i<100; i++) {
        AdaptativeHuffman_incFreq(&this, 0);
    }

    // Incremento un caracter
    for (i=0; i<100; i++) AdaptativeHuffman_incFreq(&this, 56);

    // Incremento un caracter
    for (i=0; i<15; i++) AdaptativeHuffman_incFreq(&this, 200);

    // Incremento un caracter
    for (i=0; i<4; i++) AdaptativeHuffman_incFreq(&this, 128);

    // Incremento un caracter
    for (i=0; i<99; i++) AdaptativeHuffman_incFreq(&this, 255);

    Symbol symbol;
    Symbol result = 1;
    
    // Escribo todos los caracteres en el archivo
    for (i=0, symbol=0; i<HUFFMAN_LEAF_COUNT; i++, symbol++) {
        AdaptativeHuffman_getCode_BufferedWriter(&this, symbol, writer);
    }
    BufferedWriter_closeWithEof(writer);

    // Leo los caracteres
    BufferedBitReader reader;
    BufferedBitReader_open(&reader, FILENAME, 10);
    for (symbol=0; AdaptativeHuffman_getSymbol_BufferedBitReader(&this, &result, &reader); symbol++) {
        // Los caracteres deben coincidir
        if (symbol != result) {
            printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffman_BufferedBitReaderAllChar (AdaptativeHuffmanTest) message=Error en la lectura, se leyo %d y se esperaba %d.\n", result, symbol);
        }
    }

    BufferedBitReader_close(&reader);
}


int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% AdaptativeHuffmanTest\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testAdaptativeHuffman_init (AdaptativeHuffmanTest)\n");
    testAdaptativeHuffman_init();
    printf("%%TEST_FINISHED%% time=0 testAdaptativeHuffman_init (AdaptativeHuffmanTest) \n");

    printf("%%TEST_STARTED%% testAdaptativeHuffman_incFreq (AdaptativeHuffmanTest)\n");
    testAdaptativeHuffman_incFreq();
    printf("%%TEST_FINISHED%% time=0 testAdaptativeHuffman_incFreq (AdaptativeHuffmanTest) \n");

    printf("%%TEST_STARTED%% testAdaptativeHuffman_BufferedBitReader (AdaptativeHuffmanTest)\n");
    testAdaptativeHuffman_BufferedBitReader();
    printf("%%TEST_FINISHED%% time=0 testAdaptativeHuffman_BufferedBitReader (AdaptativeHuffmanTest) \n");

    printf("%%TEST_STARTED%% testAdaptativeHuffman_getCharacterTwoInstances (AdaptativeHuffmanTest)\n");
    testAdaptativeHuffman_getCharacterTwoInstances();
    printf("%%TEST_FINISHED%% time=0 testAdaptativeHuffman_getCharacterTwoInstances (AdaptativeHuffmanTest) \n");

    printf("%%TEST_STARTED%% testAdaptativeHuffman_BufferedBitReaderAllChar (AdaptativeHuffmanTest)\n");
    testAdaptativeHuffman_BufferedBitReaderAllChar();
    printf("%%TEST_FINISHED%% time=0 testAdaptativeHuffman_BufferedBitReaderAllChar (AdaptativeHuffmanTest) \n");
    
    
    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
