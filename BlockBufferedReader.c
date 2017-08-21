#include "BlockBufferedReader.h"


int BlockBufferedReader_open(BlockBufferedReader* this, const char* filename, size_t bufferSize, size_t windowSize) {
    if (!BufferedReader_open((BufferedReader*)this, filename, bufferSize, windowSize)) return 0;
    // Mientras no se haya leido el bloque completo se lo sigue llenando a menos que se llegue al eof
    size_t readed = this->bufferLength;
    while (readed && this->windowSize + this->bufferLength < this->bufferSize) {
        readed = fread(this->buffer + this->windowSize + this->bufferLength, sizeof(unsigned char), this->bufferSize - this->bufferLength - this->windowSize, this->file);
        this->bufferLength += readed;
    }
    return 1;
}


inline int BlockBufferedReader_isOpen(BlockBufferedReader* this) {
    return BufferedReader_isOpen((BufferedReader*)this);
}


inline void BlockBufferedReader_close(BlockBufferedReader* this) {
    BufferedReader_close((BufferedReader*)this);
}


int BlockBufferedReader_readNextBlock(BlockBufferedReader* this) {
    // Si lo que se leyo del archivo es menor que el tamaño del buffer es porque se llgo al eof
    if (this->windowSize + this->bufferLength < this->bufferSize) return 0;
    size_t readed = this->bufferLength;
    this->bufferLength = 0;
    while (readed && this->windowSize + this->bufferLength < this->bufferSize) {
        readed = fread(this->buffer + this->windowSize + this->bufferLength, sizeof(unsigned char), this->bufferSize - this->windowSize - this->bufferLength, this->file);
        this->bufferLength += readed;
    }
    // Se apunta el cursor al primer byte leido
    this->cursor = this->windowSize;
    return this->bufferLength;
}


inline int BlockBufferedReader_readNextSize_t(BlockBufferedReader* this, size_t* symbol) {
    return BufferedReader_readNextSize_t(this, symbol);
}


inline int BlockBufferedReader_readNextByte(BlockBufferedReader* this, unsigned char* c) {
    // Si no hay nada en el buffer se retorna 0
    if (this->bufferLength == 0) return 0;
    // Si el cursor no se paso de la cantidad de bytes leidos
    if (this->cursor < this->bufferLength + this->windowSize) {
        // Obtengo el caracter leido
        *c = this->buffer[this->cursor];
        // Incremento el cursor
        this->cursor++;
        return 1;
    }
    return 0;
}