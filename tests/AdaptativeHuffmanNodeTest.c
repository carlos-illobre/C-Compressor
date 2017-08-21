/*
 * File:   AdaptativeHuffmanNodeTest.c
 * Author: Carlos Illobre
 *
 * Created on 22/03/2011, 23:57:46
 */

#include <stdio.h>
#include <stdlib.h>
#include "AdaptativeHuffman.h"


void testAdaptativeHuffmanLeaf_init() {
    AdaptativeHuffmanLeaf leaf;
    AdaptativeHuffmanNode* this = (AdaptativeHuffmanNode*)&leaf;
    unsigned char c = 200;
    size_t freq = 0;
    // Inicializo la hoja
    AdaptativeHuffmanLeaf_init(&leaf, c);
    // Debe ser hoja
    if (!AdaptativeHuffmanNode_isLeaf(this)) {
        printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffmanLeaf_init (AdaptativeHuffmanNodeTest) message=No es hoja\n");
        return;
    }
    // Debe tener frecuencia 1
    freq = AdaptativeHuffmanNode_getFreq(this);
    if (!AdaptativeHuffmanNode_isLeaf(this)) {
        printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffmanLeaf_init (AdaptativeHuffmanNodeTest) message=La frecuencia de la hoja es %d\n", freq);
        return;
    }
    // Debe contener el caracter asignado
    if (!AdaptativeHuffmanLeaf_hasSymbol(&leaf, c)) {
        printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffmanLeaf_init (AdaptativeHuffmanNodeTest) message=La hoja no posee el caracter %d\n", c);
        return;
    }
}


void testAdaptativeHuffmanNode_compareTo() {
    AdaptativeHuffmanLeaf leaf1;
    AdaptativeHuffmanLeaf_init(&leaf1, 0);
    AdaptativeHuffmanLeaf leaf2;
    AdaptativeHuffmanLeaf_init(&leaf2, 0);
    AdaptativeHuffmanNotLeaf notLeaf1;
    AdaptativeHuffmanNotLeaf_init(&notLeaf1, (AdaptativeHuffmanNode*)&leaf2, (AdaptativeHuffmanNode*)&leaf1);
    AdaptativeHuffmanNotLeaf notLeaf2;
    AdaptativeHuffmanNotLeaf_init(&notLeaf2, (AdaptativeHuffmanNode*)&leaf1, (AdaptativeHuffmanNode*)&leaf2);
    AdaptativeHuffmanNode* this = (AdaptativeHuffmanNode*)&leaf1;
    AdaptativeHuffmanNode* other = (AdaptativeHuffmanNode*)&leaf2;
    int result = AdaptativeHuffmanNode_compareTo(this, this);
    if (result) {
        printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffmanNode_compareTo (AdaptativeHuffmanNodeTest) message=compareTo retorna %d cuando deberia retornar %d\n", result, 0);
        return;
    }
    result = AdaptativeHuffmanNode_compareTo(this, other);
    if (result) {
        printf("%%TEST_FAILED%% time=0 testname=testAdaptativeHuffmanNode_compareTo (AdaptativeHuffmanNodeTest) message=compareTo retorna %d cuando deberia retornar %d\n", result, 0);
        return;
    }

}


int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% AdaptativeHuffmanNodeTest\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testAdaptativeHuffmanLeaf_init (AdaptativeHuffmanNodeTest)\n");
    testAdaptativeHuffmanLeaf_init();
    printf("%%TEST_FINISHED%% time=0 testAdaptativeHuffmanLeaf_init (AdaptativeHuffmanNodeTest) \n");

    printf("%%TEST_STARTED%% testAdaptativeHuffmanNotLeaf_sizeOf (AdaptativeHuffmanNodeTest)\n");
    testAdaptativeHuffmanNode_compareTo();
    printf("%%TEST_FINISHED%% time=0 testAdaptativeHuffmanNotLeaf_sizeOf (AdaptativeHuffmanNodeTest) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
