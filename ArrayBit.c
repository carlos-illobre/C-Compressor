#include "ArrayBit.h"


int ArrayBit_create(ArrayBit* this, size_t size) {
    ArrayBit_init(this);
    this->size = size;
    this->array = (char*)malloc(size*sizeof(unsigned char));
    if (this->array) {
        return 1;
    }
    return 0;
}


inline void ArrayBit_init(ArrayBit* this) {
    this->length = 0;
}


void ArrayBit_destroy(ArrayBit* this) {
    free(this->array);
    this->array = NULL;
    this->length = 0;
    this->size = 0;
}


int ArrayBit_equals(const ArrayBit* this, const ArrayBit* other) {
    if (!this->length) return 1;
    if (this->length != other->length) return 0;
    int i;
    for (i=0; i<this->length/8; i++) {
        if (this->array[i] != other->array[i]) return 0;
    }
    return this->array[i] << (8 - (this->length % 8)) == other->array[i] << (8 - (other->length % 8));
}


int ArrayBit_addLast(ArrayBit* this, int bit) {
    // Calculo la posicion del bit relativa al byte
    char mod = this->length % 8;
    unsigned char aux;
    // Si se quiere prender un bit en aux dejo todos ceros excepto en la posicion del bit a prender
    if (bit) aux = 1 << mod;
    // Si se quiere apagar un bit en aux dejo todos unos excepto en la posicion del bit a apagar
    else {
        aux = 254;
        int i;
        for (i=0; i<mod; i++) aux = (aux << 1) | 1;
    }
    // Si hay espacio para insertar un bit
    if (this->length < this->size*8) {
        this->array[this->length/8] = bit ? this->array[this->length/8] | aux : this->array[this->length/8] & aux;
        this->length++;
        return 1;
    }
    return 0;
}


inline int ArrayBit_get(const ArrayBit* this, size_t i) {
    return i < this->length ? (this->array[i/8] >> (i % 8)) & 1 : -1;
}