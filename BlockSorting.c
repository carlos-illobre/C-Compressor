#include "BlockSorting.h"


inline unsigned char BlockSorting_get(unsigned char* block, size_t i, size_t j, size_t n) {
    return (i+j < n) ? block[i+j] : block[i+j-n];
}


int BlockSorting_compareRotations(unsigned char* block, size_t i, size_t j, size_t n) {
    size_t k;
    for (k=0; k<n; k++) {
        if (BlockSorting_get(block, i, k, n) < BlockSorting_get(block, j, k, n)) return -1;
        else if (BlockSorting_get(block, i, k, n) > BlockSorting_get(block, j, k, n)) return 1;
    }
    return 0;
}


static unsigned char* tmp_block = NULL; // Argumento extra para la funcion de comparacion
static size_t tmp_n;
static int compareCode(const void * a, const void * b) {
    size_t i = *(size_t*)a;
    size_t j = *(size_t*)b;
    return BlockSorting_compareRotations(tmp_block, i, j, tmp_n);
}
static int compareDecode(const void * a, const void * b) {
    size_t i = *(size_t*)a;
    size_t j = *(size_t*)b;
    return tmp_block[i] - tmp_block[j];
}


size_t BlockSorting_code(unsigned char* block, size_t n) {
    size_t i;
    // Creo un vector de posiciones con las posiciones de los caracteres
    // iniciales de todas las rotaciones sin ordenar
    size_t* positions = (size_t*)malloc(n * sizeof(size_t));
    size_t p;
    for (p=0; p<n; p++) positions[p] = p;
    // Ordeno el vector de posiciones segun las rotaciones
    tmp_block = block;
    tmp_n = n;
    qsort(positions, n, sizeof(size_t), compareCode);
    tmp_block = NULL;
    tmp_n = 0;
    // Construyo el blockSortedString
    for (p=0; p<n; p++) {
        // Almaceno la posicion del bloque original en la matriz
        if (positions[p] == 0) i = p;
        positions[p] = positions[p] ? block[positions[p]-1] : block[n-1];
    }
    for (p=0; p<n; p++) block[p] = positions[p];
    free(positions);
    return i;
}


void BlockSorting_decode(unsigned char* block, size_t i, size_t n) {
    size_t* positions = (size_t*)malloc(n * sizeof(size_t));
    unsigned char* result = (unsigned char*)malloc(n * sizeof(unsigned char));
    size_t p;
    for (p=0; p<n; p++) positions[p] = p;
    // Ordeno el vector de posiciones segun las rotaciones
    tmp_block = block;
    qsort(positions, n, sizeof(size_t), compareDecode);
    tmp_block = NULL;
    for (p=0; p<n; p++, i=positions[i]) result[p] = block[positions[i]];
    for (p=0; p<n; p++) block[p] = result[p];
    free(result);
    free(positions);
}
