#include "AdaptativeHuffman.h"


void AdaptativeHuffman_init(AdaptativeHuffman* this) {
    unsigned nextNotLeafPos = 0;

    // Inicializo todas las hojas creando sus padres
    int i;
    for (i=0; i<HUFFMAN_LEAF_COUNT; i++) {
        // Inicializo la hoja
        AdaptativeHuffmanLeaf_init(this->leafs+i, i);
        // Si i es impar tengo dos hijos para asignar
        if (i & 1) {
            AdaptativeHuffmanNotLeaf_init(this->notLeafs+nextNotLeafPos, (AdaptativeHuffmanNode*)(this->leafs+i-1), (AdaptativeHuffmanNode*)(this->leafs+i));
            nextNotLeafPos++;
        }
    }

    // Mientras haya padres sin hijos
    for (i=0; nextNotLeafPos<HUFFMAN_LEAF_COUNT-1; nextNotLeafPos++, i+=2) {
        // los dos primeros padres seran hijos del proximo padre
        AdaptativeHuffmanNotLeaf_init(this->notLeafs+nextNotLeafPos, (AdaptativeHuffmanNode*)(this->notLeafs+i), (AdaptativeHuffmanNode*)(this->notLeafs+i+1));
    }

    // El ultimo padre es la raiz
    this->root = this->notLeafs+nextNotLeafPos-1;
    this->root->parent = NULL;
}


void AdaptativeHuffman_initUnbalanced(AdaptativeHuffman* this) {
    unsigned nextNotLeafPos = 0;
    // Creo la hoja con el simbolo mas grande
    AdaptativeHuffmanLeaf_init(this->leafs+HUFFMAN_LEAF_COUNT-1, HUFFMAN_LEAF_COUNT-1);
    // Creo la hoja con el otro simbolo mas grande
    AdaptativeHuffmanLeaf_init(this->leafs+HUFFMAN_LEAF_COUNT-2, HUFFMAN_LEAF_COUNT-2);
    // Creo el padre
    AdaptativeHuffmanNotLeaf_init(this->notLeafs+nextNotLeafPos, (AdaptativeHuffmanNode*)(this->leafs+HUFFMAN_LEAF_COUNT-1), (AdaptativeHuffmanNode*)(this->leafs+HUFFMAN_LEAF_COUNT-2));
    nextNotLeafPos++;

    // Inicializo todas las hojas creando sus padres
    int i;
    for (i=2; i<HUFFMAN_LEAF_COUNT; i++) {
        // Inicializo la proxima hoja
        AdaptativeHuffmanLeaf_init(this->leafs+HUFFMAN_LEAF_COUNT-i-1, HUFFMAN_LEAF_COUNT-i-1);
        AdaptativeHuffmanNode_setFreq((AdaptativeHuffmanNode*)this->leafs+HUFFMAN_LEAF_COUNT-i-1, i);
        // Creo el padre
        AdaptativeHuffmanNotLeaf_init(this->notLeafs+nextNotLeafPos, (AdaptativeHuffmanNode*)(this->leafs+HUFFMAN_LEAF_COUNT-i-1), (AdaptativeHuffmanNode*)(this->notLeafs+nextNotLeafPos-1));
        nextNotLeafPos++;
    }

    // El ultimo padre es la raiz
    this->root = this->notLeafs+nextNotLeafPos-1;
    this->root->parent = NULL;
}


int AdaptativeHuffman_getCode_ArrayBit(const AdaptativeHuffman* this, Symbol symbol, ArrayBit* code) {
    size_t freq = 0;
    ArrayBit_init(code);
    if (symbol < HUFFMAN_LEAF_COUNT) {
        // Obtengo la hoja con el caracter
        AdaptativeHuffmanNode* node = (AdaptativeHuffmanNode*)(this->leafs + symbol);
        freq = AdaptativeHuffmanNode_getFreq(node);
        
        // Mientras no se llegue a la raiz
        for (; !AdaptativeHuffmanNode_isRoot(node); node=(AdaptativeHuffmanNode*)AdaptativeHuffmanNode_getParent(node)) {
            // Si es hijo izquierdo agrego un cero sino agrego un uno
            ArrayBit_addLast(code, !AdaptativeHuffmanNotLeaf_isLeftChild(node));
        }
    }
    return freq;
}


int AdaptativeHuffman_getCode_BufferedWriter(const AdaptativeHuffman* this, Symbol symbol, BufferedWriter* buffer) {
    int bits = 0;
    size_t freq = 0;
    size_t array = 0;
    if (symbol < HUFFMAN_LEAF_COUNT) {
        // Obtengo la hoja con el caracter
        AdaptativeHuffmanNode* node = (AdaptativeHuffmanNode*)(this->leafs + symbol);
        freq = AdaptativeHuffmanNode_getFreq(node);
        // Mientras no se llegue a la raiz
        for (; !AdaptativeHuffmanNode_isRoot(node); node=(AdaptativeHuffmanNode*)AdaptativeHuffmanNode_getParent(node)) {
            array = array << 1;
            // Si es hijo izquierdo agrego un cero sino agrego un uno
            if (!AdaptativeHuffmanNotLeaf_isLeftChild(node)) array++;
            bits++;
        }
        for (; bits; bits--) {
            BufferedWriter_writeBit(buffer, array & 1);
            array = array >> 1;
        }
    }
    return freq;
}


Symbol AdaptativeHuffman_getSymbol_ArrayBit(const AdaptativeHuffman* this, const ArrayBit* code) {
    AdaptativeHuffmanNotLeaf* parent;
    size_t i;
    for (i=code->length-1, parent=this->root; i>0; i--) {
        parent = (AdaptativeHuffmanNotLeaf*)(ArrayBit_get(code, i) ? parent->rightChild : parent->leftChild);
    }
    return ((AdaptativeHuffmanLeaf*)(ArrayBit_get(code, i) ? parent->rightChild : parent->leftChild))->symbol;
}


int AdaptativeHuffman_getSymbol_BufferedBitReader(const AdaptativeHuffman* this, Symbol* symbol, BufferedBitReader* code) {
    AdaptativeHuffmanNode* node = (AdaptativeHuffmanNode*)this->root;
    // Mientras no se haya llegado a una hoja
    while (!AdaptativeHuffmanNode_isLeaf(node)) {
        // Leo un bit
        switch (BufferedBitReader_readBit(code)) {
            // Si es cero voy a la rama izquierda
            case 0:
                node = ((AdaptativeHuffmanNotLeaf*)node)->leftChild;
                break;
            // Si es uno voy a la rama derecha
            case 1:
                node = ((AdaptativeHuffmanNotLeaf*)node)->rightChild;
                break;
            // Si se llego al fin de archivo no hay nada para leer
            default:
                return 0;
        }
    }
    *symbol = ((AdaptativeHuffmanLeaf*)node)->symbol;
    return 1;
}


int AdaptativeHuffman_incFreq(AdaptativeHuffman* this, Symbol symbol) {
    // Almacena los nodos del arbol ordenados por nivel empezando por la raiz
    AdaptativeHuffmanNode* array[HUFFMAN_NODE_COUNT];
    // Cantidad de nodos que hay guardados en el array
    unsigned arrayLength = 0;
    // Proximo nodo del array a leer
    unsigned arrayIterator = 0;

    AdaptativeHuffmanNode* cursor;
    AdaptativeHuffmanNode* node = NULL;
    // Recorro el arbol por nivel hasta encontrar la hoja con el caracter buscado
    // Guardo la raiz
    array[arrayLength++] = (AdaptativeHuffmanNode*)this->root;

    do {
        // Desacolo
        cursor = array[arrayIterator++];
        //cursor = Queue_pop(&q);
        // Inserto el nodo al principio de la lista
        //LinkedList_addFirst(&l, cursor);
        // Si el nodo no es una hoja
        if (!AdaptativeHuffmanNode_isLeaf(cursor)) {
            // Acolo el hijo derecho
            array[arrayLength++] = AdaptativeHuffmanNotLeaf_getRightChild((AdaptativeHuffmanNotLeaf*)cursor);
            //Queue_push(&q, DynamicHuffmanNotLeaf_getRightChild((DynamicHuffmanNotLeaf*)cursor));
            // Acolo el hijo izquierdo
            array[arrayLength++] = AdaptativeHuffmanNotLeaf_getLeftChild((AdaptativeHuffmanNotLeaf*)cursor);
            //Queue_push(&q, DynamicHuffmanNotLeaf_getLeftChild((DynamicHuffmanNotLeaf*)cursor));
        // Si es una hoja y es la hoja que contiene al caracter
        } else if (AdaptativeHuffmanLeaf_hasSymbol((AdaptativeHuffmanLeaf*)cursor, symbol)) {
            node = cursor;
        }
    // Mientras la cola no este vacia y no se haya encontrado la hoja con el caracter
    } while (arrayIterator < arrayLength && !node);
    //} while (!Queue_isEmpty(&q) && !node);

    // Ya no hace falta la cola
    arrayIterator--;
    //Queue_destroy(&q);
    // Si el caracter se encontro en el arbol
    if (node) {
        node = array[arrayIterator--];
        //node = (AdaptativeHuffmanNode*)LinkedList_removeFirst(&l);
        cursor = array[arrayIterator--];
        //cursor = (AdaptativeHuffmanNode*)LinkedList_removeFirst(&l);
        // Incremento la frecuencia del caracter
        AdaptativeHuffmanNode_incFreq(node);
        // Si el nodo tiene una frecuencia mayor a la de su siguiente entonces
        // reordeno el arbol, sino no hace falta
        while (cursor && AdaptativeHuffmanNode_isFreqGreaterThan(node, cursor)) {
            // Hay que intercambiar en el arbol la hoja con el caracter buscado con
            // el nodo anterior al de mayor o igual frecuencia
            // Busco el nodo con el que hay que intercambiar a la hoja
            // Mientras la hoja a intercambiar tenga mayor frecuencia que el primer nodo de la lista
            while (AdaptativeHuffmanNode_isFreqGreaterThan(node, array[arrayIterator])) {
            //while (DynamicHuffmanNode_isFreqGreaterThan(node, (AdaptativeHuffmanNode*)LinkedList_getFirst(&l))) {
                // A cursor le asigno el primer nodo de la lista quitandolo de la lista
                cursor = array[arrayIterator--];
                //cursor = (AdaptativeHuffmanNode*)LinkedList_removeFirst(&l);
            }
            // En cursor quedo el nodo previo al nodo con frecuencia mayor o igual a la hoja
            // Intercambio los nodos
            AdaptativeHuffmanNode_swap(node, cursor);
            // Recalculo las frecuencias de los padres
            AdaptativeHuffmanNode_calculateFreq(AdaptativeHuffmanNode_getParent(cursor));
            AdaptativeHuffmanNode_calculateFreq(AdaptativeHuffmanNode_getParent(node));
            // Busco si algun padre tiene frecuencia mayor a un previo.
            node = (AdaptativeHuffmanNode*)AdaptativeHuffmanNode_getParent(cursor);
            // Elimino de la lista todos los nodos previos a node
            while (array[arrayIterator--] != node);
            //while (LinkedList_removeFirst(&l) != node);
            cursor = arrayIterator < HUFFMAN_NODE_COUNT ? array[arrayIterator--] : NULL;
            //cursor = (AdaptativeHuffmanNode*)LinkedList_removeFirst(&l);
        }
        // Incremento la frecuencia de los padres
        AdaptativeHuffmanNode_calculateFreq(AdaptativeHuffmanNode_getParent(node));
        //LinkedList_destroy(&l);
        return 1;
    } else return 0;
}
