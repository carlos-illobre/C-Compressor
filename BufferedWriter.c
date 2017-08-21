#include <stdlib.h>
#include "BufferedWriter.h"


size_t BufferedWriter_sizeof() {
    return sizeof(FILE*) + sizeof(unsigned char*) + sizeof(size_t) + sizeof(size_t);
}


BufferedWriter* BufferedWriter_open(const char* filename, size_t bufferSize) {
    // Creo el objeto
    BufferedWriter* this = (BufferedWriter*)malloc(BufferedWriter_sizeof());
    // Si no hay memoria no se puede crear
    if (!this) return NULL;
    // Creo el buffer
    this->buffer = (unsigned char*)malloc(bufferSize * sizeof(unsigned char));
    // Si no hay memoria
    if (!this->buffer) {
        // Libero la memoria del objeto
        free(this);
        // El objeto no se pudo crear
        return NULL;
    }
    // Asigno el tamaño fisico del buffer
    this->bufferSize = bufferSize;
    // Se abre el archivo
    this->file = fopen(filename, "wb");
    // Si el archivo no se puede abrir
    if (!this->file) {
        // Se libera la memoria reservada para el buffer
        free(this->buffer);
        // Se libera la memoria del objeto
        free(this);
        // No se pudo crear el buffer
        return NULL;
    }
    // Se apunta el cursor al primer byte
    this->cursor = 0;
    // Se apunta el cursor al primer bit
    this->bitCursor = 0;
    return this;
}


void BufferedWriter_close(BufferedWriter* this) {
    // flush
    fwrite(this->buffer, sizeof(unsigned char), this->cursor, this->file);
    // Se cierra el archivo
    fclose(this->file);
    // Se libera la memoria reservada para el buffer
    free(this->buffer);
    // Se libera la memoria del objeto
    free(this);
}


void BufferedWriter_writeByte(BufferedWriter* this, unsigned char c) {
    // Escribo el caracter en el buffer
    this->buffer[this->cursor] = c;
    // Incremento el cursor
    this->cursor++;
    // Si el cursor se paso de la cantidad de bytes leidos
    if (this->cursor == this->bufferSize) {
        // Se escribe el buffer en el archivo
        fwrite(this->buffer, sizeof(unsigned char), this->bufferSize, this->file);
        // Se hace apuntar a cursor al primer byte del buffer
        this->cursor = 0;
    }
}


void BufferedWriter_writeByteArray(BufferedWriter* this, unsigned char* array, size_t n) {
    size_t i;
    for (i=0; i<n; i++) BufferedWriter_writeByte(this, array[i]);
}


void BufferedWriter_writeSize_t(BufferedWriter* this, size_t symbol) {
    unsigned char* aux = (unsigned char*)(&symbol);
    int i;
    for (i=1; i<=sizeof(size_t); i++) BufferedWriter_writeByte(this, aux[sizeof(size_t)-i]);
}


void BufferedWriter_writeBit(BufferedWriter* this, int bit) {
    // Escribo un cero
    this->buffer[this->cursor] = this->buffer[this->cursor] << 1;
    // Si hay que escribir un 1
    if (bit) this->buffer[this->cursor]++;
    // Incremento bitCursor
    this->bitCursor++;
    // Si se escribieron los 8 bit
    if (this->bitCursor == 8) {
        // Incremento el cursor
        this->cursor++;
        // El cursor de bits ahora es cer
        this->bitCursor = 0;
    }
    // Si el cursor se paso de la cantidad de bytes leidos
    if (this->cursor == this->bufferSize) {
        // Se escribe el buffer en el archivo
        fwrite(this->buffer, sizeof(unsigned char), this->bufferSize, this->file);
        // Se hace apuntar a cursor al primer byte del buffer
        this->cursor = 0;
    }
}


void BufferedWriter_write8Bit(BufferedWriter* this, unsigned char bits) {
    char i;
    for (i=0; i<8; i++, bits <<= 1) {
        BufferedWriter_writeBit(this, bits & 128);
    }
}


void BufferedWriter_writeUnary(BufferedWriter* this, size_t n) {
    size_t i;
    // Escribo n-1 unos
    for (i=0; i<n-1; i++) BufferedWriter_writeBit(this, 1);
    // Escribo el cero final
    BufferedWriter_writeBit(this, 0);
}


void BufferedWriter_writeBinary(BufferedWriter* this, size_t n) {
    if (n) {
        int bits = floorLog2(n);
        size_t one = 1 << bits;
        for (bits++; bits; bits--) {
            BufferedWriter_writeBit(this, n && one);
            n = n << 1;
        }
    } else BufferedWriter_writeBit(this, 0);
}


void BufferedWriter_writePrefix(BufferedWriter* this, size_t n, size_t max) {
    size_t i;
    // Escribo n unos (admite n=0)
    for (i=0; i<n; i++) BufferedWriter_writeBit(this, 1);
    // Escribo el cero final unicamente si no es el valor maximo
    if (n != max) BufferedWriter_writeBit(this, 0);
}


int floorLog2(unsigned int n) {
    int pos = 0;
    if (n >= 1<<16) { n >>= 16; pos += 16; }
    if (n >= 1<< 8) { n >>=  8; pos +=  8; }
    if (n >= 1<< 4) { n >>=  4; pos +=  4; }
    if (n >= 1<< 2) { n >>=  2; pos +=  2; }
    if (n >= 1<< 1) {           pos +=  1; }
    return n ? pos : -1;
}


void BufferedWriter_writeGamma(BufferedWriter* this, size_t n) {
    if (n == 1) BufferedWriter_writeBit(this, 0);
    else {
        // Escribo la cantidad de bits en unario
        int bits = floorLog2(n);
        BufferedWriter_writeUnary(this, bits+1);
        size_t one = 1 << (bits-1);
        int i;
        for (i=0; i<bits; i++) {
            BufferedWriter_writeBit(this, one & (n << i));
        }
    }
}


void BufferedWriter_writeDelta(BufferedWriter* this, size_t n) {
    if (n == 1) BufferedWriter_writeBit(this, 0);
    else {
        // Escribo la cantidad de bits en gamma
        int bits = floorLog2(n);
        BufferedWriter_writeGamma(this, bits+1);
        size_t one = 1 << (bits-1);
        int i;
        for (i=0; i<bits; i++) {
            BufferedWriter_writeBit(this, one & (n << i));
        }
    }
}


void BufferedWriter_closeWithEof(BufferedWriter* this) {
    // Agrego un 1
    BufferedWriter_writeBit(this, 1);
    // Agrego tantos ceros como hagan falta para completar el byte
    while (this->bitCursor) {
        BufferedWriter_writeBit(this, 0);
    }
    // Cierro el archivo
    BufferedWriter_close(this);
}


