#include "AdaptativeHuffmanNode.h"


inline void AdaptativeHuffmanLeaf_init(AdaptativeHuffmanLeaf* this, Symbol symbol) {
    this->symbol = symbol;
    this->freq = 3;
}


inline void AdaptativeHuffmanNotLeaf_init(AdaptativeHuffmanNotLeaf* this, AdaptativeHuffmanNode* leftChild, AdaptativeHuffmanNode* rightChild) {
    this->leftChild = leftChild;
    this->rightChild = rightChild;
    leftChild->parent = rightChild->parent = this;
    this->freq = 0;
    AdaptativeHuffmanNode_addFreq((AdaptativeHuffmanNode*)this, AdaptativeHuffmanNode_getFreq(leftChild) + AdaptativeHuffmanNode_getFreq(rightChild));
}


inline int AdaptativeHuffmanNode_isLeaf(const AdaptativeHuffmanNode* this) {
    return this->freq & 1;
}


inline size_t AdaptativeHuffmanNode_getFreq(const AdaptativeHuffmanNode* this) {
    return this->freq/2;
}


inline size_t AdaptativeHuffmanNode_setFreq(AdaptativeHuffmanNode* this, size_t freq) {
    this->freq = (this->freq & 1) + (freq << 1);
}


inline void AdaptativeHuffmanNode_addFreq(AdaptativeHuffmanNode* this, size_t freq) {
    this->freq += (freq << 1);
}


inline void AdaptativeHuffmanNode_incFreq(AdaptativeHuffmanNode* this) {
    AdaptativeHuffmanNode_addFreq(this, 1);
}


inline int AdaptativeHuffmanLeaf_hasSymbol(const AdaptativeHuffmanLeaf* this, Symbol symbol) {
    return this->symbol == symbol;
}


inline int AdaptativeHuffmanNode_compareTo(const AdaptativeHuffmanNode* this, const AdaptativeHuffmanNode* other) {
    return (other->freq >> 1) - (this->freq >> 1);
}


inline int AdaptativeHuffmanNode_isFreqGreaterThan(const AdaptativeHuffmanNode* this, const AdaptativeHuffmanNode* other) {
    return AdaptativeHuffmanNode_compareTo(this, other) < 0;
}


inline AdaptativeHuffmanNode* AdaptativeHuffmanNotLeaf_getLeftChild(const AdaptativeHuffmanNotLeaf* this) {
    return this->leftChild;
}


inline AdaptativeHuffmanNode* AdaptativeHuffmanNotLeaf_getRightChild(const AdaptativeHuffmanNotLeaf* this) {
    return this->rightChild;
}


inline AdaptativeHuffmanNotLeaf* AdaptativeHuffmanNode_getParent(const AdaptativeHuffmanNode* this) {
    return this->parent;
}


inline int AdaptativeHuffmanNotLeaf_isLeftChild(const AdaptativeHuffmanNode* this) {
    return this->parent && AdaptativeHuffmanNotLeaf_getLeftChild(this->parent) == this;
}


void AdaptativeHuffmanNode_swap(AdaptativeHuffmanNode* this, AdaptativeHuffmanNode* other) {
    // Si this es hijo izquierdo
    if (AdaptativeHuffmanNotLeaf_isLeftChild(this)) {
        // Al padre de this le asigno other como hijo izquierdo
        AdaptativeHuffmanNode_getParent(this)->leftChild = other;
    // Si this es hijo derecho (nunca va a ser la raiz)
    } else {
        // Al padre de this le asigno other como hijo derecho
        AdaptativeHuffmanNode_getParent(this)->rightChild = other;
    }

    // Si other es hijo izquierdo
    if (AdaptativeHuffmanNotLeaf_isLeftChild(other)) {
        // Al padre de other le asigno this como hijo izquierdo
        AdaptativeHuffmanNode_getParent(other)->leftChild = this;
    // Si other es hijo derecho (nunca va a ser la raiz)
    } else {
        // Al padre de other le asigno this como hijo derecho
        AdaptativeHuffmanNode_getParent(other)->rightChild = this;
    }

    // Intercambio los padres
    AdaptativeHuffmanNotLeaf* parent = AdaptativeHuffmanNode_getParent(this);
    this->parent = other->parent;
    other->parent = parent;
}


void AdaptativeHuffmanNode_calculateFreq(AdaptativeHuffmanNotLeaf* this) {
    while (this) {
        AdaptativeHuffmanNode_setFreq((AdaptativeHuffmanNode*)this, AdaptativeHuffmanNotLeaf_getLeftChildFreq(this) + AdaptativeHuffmanNotLeaf_getRightChildFreq(this));
        this = AdaptativeHuffmanNode_getParent((AdaptativeHuffmanNode*)this);
    }
}


inline int AdaptativeHuffmanNode_isRoot(const AdaptativeHuffmanNode* this) {
    return !this->parent;
}


inline size_t AdaptativeHuffmanNotLeaf_getLeftChildFreq(const AdaptativeHuffmanNotLeaf* this) {
    return AdaptativeHuffmanNode_getFreq(AdaptativeHuffmanNotLeaf_getLeftChild(this));
}


inline size_t AdaptativeHuffmanNotLeaf_getRightChildFreq(const AdaptativeHuffmanNotLeaf* this) {
    return AdaptativeHuffmanNode_getFreq(AdaptativeHuffmanNotLeaf_getRightChild(this));
}
