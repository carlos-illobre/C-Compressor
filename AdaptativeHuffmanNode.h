/* 
 * File:   AdaptativeHuffmanNode.h
 * Author: Carlos Illobre
 *
 * Created on 21 de marzo de 2011, 22:59
 */

#ifndef ADAPTATIVEHUFFMANNODE_H
#define	ADAPTATIVEHUFFMANNODE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdlib.h>

    struct AdaptativeHuffmanNotLeaf;

    typedef struct AdaptativeHuffmanNode {
        size_t freq;
        struct AdaptativeHuffmanNotLeaf* parent;
    } AdaptativeHuffmanNode;

    typedef struct AdaptativeHuffmanNotLeaf {
        size_t freq;
        struct AdaptativeHuffmanNotLeaf* parent;
        AdaptativeHuffmanNode* leftChild;
        AdaptativeHuffmanNode* rightChild;
    } AdaptativeHuffmanNotLeaf;

    typedef int Symbol;
    //typedef unsigned char Symbol;
    typedef struct AdaptativeHuffmanLeaf {
        size_t freq;
        struct AdaptativeHuffmanNotLeaf* parent;
        Symbol symbol;
    } AdaptativeHuffmanLeaf;

    /**
     * Inicializa a una hoja
     * @param this
     * @param symbol
     */
    inline void AdaptativeHuffmanLeaf_init(AdaptativeHuffmanLeaf* this, Symbol symbol);

    /**
     * Inicializa un nodo no hoja
     * @param this
     * @param leftChild
     * @param rightChild
     */
    inline void AdaptativeHuffmanNotLeaf_init(AdaptativeHuffmanNotLeaf* this, AdaptativeHuffmanNode* leftChild, AdaptativeHuffmanNode* rightChild);
    
    /**
     * @param this Nodo del arbol de huffman
     * @return true si el nodo es una hoja y false en otro caso
     */
    inline int AdaptativeHuffmanNode_isLeaf(const AdaptativeHuffmanNode* this);

    /**
     *
     * @param this
     * @return La frecuencia del nodo
     */
    inline size_t AdaptativeHuffmanNode_getFreq(const AdaptativeHuffmanNode* this);

    inline size_t AdaptativeHuffmanNode_setFreq(AdaptativeHuffmanNode* this, size_t freq);

    /**
     * Suma una frecuencia al nodo
     * @param this
     * @param freq
     */
    inline void AdaptativeHuffmanNode_addFreq(AdaptativeHuffmanNode* this, size_t freq);

    /**
     * Incrementa en uno la frecuencia del nodo
     * @param this
     */
    inline void AdaptativeHuffmanNode_incFreq(AdaptativeHuffmanNode* this);

    /**
     * 
     * @param this
     * @param c
     * @return true si la hoja tiene el caracter c, false en otro caso
     */
    inline int AdaptativeHuffmanLeaf_hasSymbol(const AdaptativeHuffmanLeaf* this, Symbol symbol);

    inline int AdaptativeHuffmanNode_compareTo(const AdaptativeHuffmanNode* this, const AdaptativeHuffmanNode* other);

    /** @return true si la frecuencia de this es mayor que la frecuencia de other. */
    inline int AdaptativeHuffmanNode_isFreqGreaterThan(const AdaptativeHuffmanNode* this, const AdaptativeHuffmanNode* other);

    inline AdaptativeHuffmanNode* AdaptativeHuffmanNotLeaf_getLeftChild(const AdaptativeHuffmanNotLeaf* this);
    inline AdaptativeHuffmanNode* AdaptativeHuffmanNotLeaf_getRightChild(const AdaptativeHuffmanNotLeaf* this);
    inline AdaptativeHuffmanNotLeaf* AdaptativeHuffmanNode_getParent(const AdaptativeHuffmanNode* this);
    
    /**
     * 
     * @param this
     * @return true si es hijo izquierdo y false en otro caso
     */
    inline int AdaptativeHuffmanNotLeaf_isLeftChild(const AdaptativeHuffmanNode* this);

    inline size_t AdaptativeHuffmanNotLeaf_getLeftChildFreq(const AdaptativeHuffmanNotLeaf* this);
    inline size_t AdaptativeHuffmanNotLeaf_getRightChildFreq(const AdaptativeHuffmanNotLeaf* this);

    /**
     * Intercambia los valores de los nodos
     * @param this
     * @param other
     */
    void AdaptativeHuffmanNode_swap(AdaptativeHuffmanNode* this, AdaptativeHuffmanNode* other);

    /**
     * Incrementa en 1 la frecuencia de el padre de this y todos sus ancestros
     * hasta la raiz
     * @param this
     */
    void AdaptativeHuffmanNode_calculateFreq(AdaptativeHuffmanNotLeaf* this);

    inline int AdaptativeHuffmanNode_isRoot(const AdaptativeHuffmanNode* this);

#ifdef	__cplusplus
}
#endif

#endif	/* ADAPTATIVEHUFFMANNODE_H */

