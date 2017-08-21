/* 
 * File:   AdaptativeHuffman.h
 * Author: Carlos Illobre
 *
 * Created on 21 de marzo de 2011, 23:05
 */

#ifndef ADAPTATIVEHUFFMAN_H
#define	ADAPTATIVEHUFFMAN_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "AdaptativeHuffmanNode.h"
#include "ArrayBit.h"
#include "BufferedWriter.h"
#include "BufferedBitReader.h"
#include <limits.h>

// Cantidad de hojas que tiene el arbol de huffman
#define HUFFMAN_LEAF_COUNT UCHAR_MAX*2
// Cantidad de nodos que tiene un arbol de huffman
#define HUFFMAN_NODE_COUNT HUFFMAN_LEAF_COUNT*2-1
// Maximo valor del simbolo que no es caracter
#define HUFFMAN_MAX_LENGTH HUFFMAN_LEAF_COUNT-UCHAR_MAX-1


    typedef struct AdaptativeHuffman {
        AdaptativeHuffmanNotLeaf* root;
        AdaptativeHuffmanLeaf leafs[HUFFMAN_LEAF_COUNT];
        AdaptativeHuffmanNotLeaf notLeafs[HUFFMAN_LEAF_COUNT-1];
    } AdaptativeHuffman;

    /**
     * Inicializa un arbol de huffman
     */
    void AdaptativeHuffman_init(AdaptativeHuffman* this);

    void AdaptativeHuffman_initUnbalanced(AdaptativeHuffman* this);

    /**
     *
     * @param this Arbol de huffman
     * @param symbol Simbolo del cual se quiere conocer su codigo
     * @param code El codigo de huffman del caracter c en el arbol de huffman
     * @return La frecuencia del caracter hasta el momento
     */
    int AdaptativeHuffman_getCode_ArrayBit(const AdaptativeHuffman* this, Symbol symbol, ArrayBit* code);

    /**
     * Escribe en el archivo el codigo del caracter c
     * @param this Arbol de huffman
     * @param symbol Simbolo del cual se quiere conocer su codigo
     * @param code El codigo de huffman del caracter c en el arbol de huffman
     * @return La frecuencia del caracter hasta el momento
     */
    int AdaptativeHuffman_getCode_BufferedWriter(const AdaptativeHuffman* this, Symbol symbol, BufferedWriter* code);

    /**
     *
     * @param this
     * @param code
     * @return El simbolo correspondiente al codigo
     */
    Symbol AdaptativeHuffman_getSymbol_ArrayBit(const AdaptativeHuffman* this, const ArrayBit* code);

    /**
     *
     * @param this
     * @param code
     * @return 1 si se pudo leer el simbolo o 0 si no hay mas simbolos para leer
     */
    int AdaptativeHuffman_getSymbol_BufferedBitReader(const AdaptativeHuffman* this, Symbol* symbol, BufferedBitReader* code);

    /**
     * Modifica el arbol de modo que el caracter c incremente en uno su frecuencia
     * @param this Arbol de huffman
     * @param symbol Simbolo que se quiere incrementar su frecuencia
     * @return true si se encontro el caracter y false en otro caso
     */
    int AdaptativeHuffman_incFreq(AdaptativeHuffman* this, Symbol symbol);




#ifdef	__cplusplus
}
#endif

#endif	/* ADAPTATIVEHUFFMAN_H */
