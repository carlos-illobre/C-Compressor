#include "BufferedReader.h"


int BufferedReader_open(BufferedReader* this, const char* filename, size_t bufferSize, size_t windowSize) {
    this->file = NULL;
    // Creo el buffer
    this->buffer = (unsigned char*)malloc(bufferSize * sizeof(unsigned char));
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
    this->windowSize = windowSize;
    // Se llena el buffer
    this->bufferLength = fread(this->buffer + this->windowSize, sizeof(unsigned char), this->bufferSize - this->windowSize, this->file);
    // Se apunta el cursor al primer byte leido
    this->cursor = this->windowSize;
    return 1;
}


inline int BufferedReader_isOpen(BufferedReader* this) {
    return this->file != NULL;
}


inline int BufferedReader_isEof(BufferedReader* this) {
    return !this->bufferLength;
}


void BufferedReader_close(BufferedReader* this) {
    // Se cierra el archivo
    fclose(this->file);
    this->file = NULL;
    // Se libera la memoria reservada para el buffer
    free(this->buffer);
    this->buffer = NULL;
}


int BufferedReader_readNextByte(BufferedReader* this, unsigned char* c) {
    // Si no hay nada en el buffer se retorna 0
    if (this->bufferLength == 0) return 0;
    // Obtengo el caracter leido
    *c = this->buffer[this->cursor];
    // Incremento el cursor
    this->cursor++;
    // Si el cursor se paso de la cantidad de bytes leidos
    if (this->cursor >= this->bufferLength + this->windowSize) {
        this->bufferLength = fread(this->buffer + this->windowSize, sizeof(unsigned char), this->bufferSize - this->windowSize, this->file);
        // Se apunta el cursor al primer byte leido
        this->cursor = this->windowSize;
    }
    return 1;
}


int BufferedReader_readNextSize_t(BufferedReader* this, size_t* symbol) {
    unsigned char* aux = (unsigned char*)symbol;
    int i;
    for (i=1; i<=sizeof(size_t) && BufferedReader_readNextByte(this, aux+sizeof(size_t)-i); i++);
    return i > sizeof(size_t);
}
