#include "BufferedBitReader.h"
#include "BufferedReader.h"


int BufferedBitReader_open(BufferedBitReader* this, const char* filename, size_t bufferSize) {
    this->file = NULL;
    // Creo el buffer con una posicion extra para el manejo de eof para la lectura de bits
    this->buffer = (unsigned char*)malloc(1 + bufferSize * sizeof(unsigned char));
    // Si no hay memoria el objeto no se pudo crear
    if (!this->buffer) return 0;
    // Asigno el tamaño fisico de lectura del buffer
    this->bufferSize = bufferSize;
    // Se abre el archivo
    this->file = fopen(filename, "rb");
    // Si el archivo no se puede abrir
    if (!this->file) {
        // Se libera la memoria reservada para el buffer
        free(this->buffer);
        // No se pudo crear el buffer
        return 0;
    }
    // Se llena el buffer
    this->bufferLength = fread(this->buffer, sizeof(unsigned char), this->bufferSize +1, this->file);
    // Se apunta el cursor al primer byte
    this->cursor = 0;
    // Se apunta el cursor al primer bit
    this->bitCursor = 0;
    this->byteLength = 8;
    return 1;
}


inline int BufferedBitReader_isOpen(BufferedBitReader* this) {
    return BufferedReader_isOpen((BufferedReader*)this);
}


inline void BufferedBitReader_close(BufferedBitReader* this) {
    BufferedReader_close((BufferedReader*)this);
}


int BufferedBitReader_readBit(BufferedBitReader* this) {
    // Si no hay nada en el buffer se retorna -1
    if (this->bufferLength == 0 || this->byteLength == 0) return -1;
    // Obtengo el bit
    int bit = this->buffer[this->cursor] & 128 ? 1 : 0;
    // Me posiciono en el siguiente bit
    this->buffer[this->cursor] = this->buffer[this->cursor] << 1;
    // Incremento el cursor del bit
    this->bitCursor++;
    // Si se leyeron todos los bits validos del byte
    if (this->bitCursor == this->byteLength) {
        // Si se estaba leyendo el byte con el eof ya no hay mas bits validos en un byte
        if (this->byteLength < 8) this->byteLength = 0;
        this->bitCursor = 0;
        this->cursor++;
    }
    // Si el cursor apunta al ultimo byte leido del archivo
    if (this->cursor == this->bufferLength-1) {
        // Copio el ultimo byte al principio del buffer
        this->buffer[0] = this->buffer[this->bufferLength-1];
        // Se lee otro bloque del archivo y se lo coloca en el buffer sin sobreescribir la primera posicion
        this->bufferLength = 1 + fread(this->buffer+1, sizeof(unsigned char), this->bufferSize, this->file);
        // Se hace apuntar a cursor al primer byte del buffer
        this->cursor = 0;
    }
    // Si no se leyo nada del archivo entonces en la primera posicion del buffer esta el eof
    if (this->bufferLength == 1 && this->byteLength == 8) {
        // Calculo la cantidad de bits validos del primer byte del buffer
        unsigned char aux = this->buffer[this->cursor];
        for (this->byteLength=7; !(aux & 1); this->byteLength--) aux = aux >> 1;
    }
    return bit;
}


int BufferedBitReader_read8Bit(BufferedBitReader* this, unsigned char* c) {
    int i;
    int bit;
    for (i=0; i<8; i++) {
        bit = BufferedBitReader_readBit(this);
        if (bit == -1) return i;
        (*c) = (*c) << 1;
        if (bit) (*c) += 1;
    }
    return i;
}


size_t BufferedBitReader_readUnary(BufferedBitReader* this) {
    size_t n;
    int bit = 1;
    // Leo unos hasta encontrar un cero
    for (n=0; bit; n++) {
        bit = BufferedBitReader_readBit(this);
        if (bit == -1) return 0;
    }
    return n;
}


size_t BufferedBitReader_readPrefix(BufferedBitReader* this, size_t max) {
    size_t n = 0;
    int bit = BufferedBitReader_readBit(this);
    while (bit==1) {
        n++;
        if (n == max) return max;
        bit = BufferedBitReader_readBit(this);
    }
    return bit == -1 ? 0 : n;
}


size_t BufferedBitReader_readGamma(BufferedBitReader* this) {
    size_t n = 1;
    size_t bits = BufferedBitReader_readUnary(this);
    int bit;
    if (!bits) return 0;
    for (bits--; bits; bits--) {
        n = n << 1;
        bit = BufferedBitReader_readBit(this);
        switch (bit) {
            case 1:
                n++;
                break;
            case -1:
                return 0;
        }
    }
    return n;
}


size_t BufferedBitReader_readDelta(BufferedBitReader* this) {
    size_t n = 1;
    size_t bits = BufferedBitReader_readGamma(this);
    int bit;
    if (!bits) return 0;
    for (bits--; bits; bits--) {
        n = n << 1;
        bit = BufferedBitReader_readBit(this);
        switch (bit) {
            case 1:
                n++;
                break;
            case -1:
                return 0;
        }
    }
    return n;
}

