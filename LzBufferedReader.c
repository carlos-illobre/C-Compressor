#include "LzBufferedReader.h"


int LzBufferedReader_open(LzBufferedReader* this, const char* filename, size_t bufferSize, size_t windowSize) {
    if (BufferedReader_open(&(this->reader), filename, bufferSize, windowSize)) {
        // A los caracteres de la ventana los inicializo con ceros
        size_t i;
        for (i=0; i<this->reader.windowSize; i++) this->reader.buffer[i] = 0;
        return 1;
    }
    return 0;
}


void LzBufferedReader_close(LzBufferedReader* this) {
    BufferedReader_close(&(this->reader));
}


// Tengo que asegurarme que lo que leo del archivo (bufferLength) sea al menos tan grande como la ventana
int LzBufferedReader_readNextLengthDistance(LzBufferedReader* this, size_t* length, size_t* distance, size_t minLength) {
    size_t matchDistance, matchLength, cursorPos = this->reader.cursor;
    unsigned char c;
    *length = *distance = 0;
    // Leo el caracter
    if (BufferedReader_isEof(&(this->reader))) return 0;
    // Busco la primera posicion de la ventana donde hay match
    for (matchDistance=0; matchDistance < this->reader.windowSize; matchDistance++) {
        // Si hubo match
        if (this->reader.buffer[this->reader.cursor] == this->reader.buffer[this->reader.cursor-matchDistance-1]) {
            // mientras el caracter leido sea igual al de la posicion de la memoria donde hubo match
            for (matchLength=0; this->reader.buffer[this->reader.cursor] == this->reader.buffer[this->reader.cursor-matchDistance-1] && LzBufferedReader_readNextByte(this, &c); matchLength++);
        } else matchLength = 0;
        // Si la longitud de match es mayor a la previamente detectada se guardan los valores de posicion y longitud
        if (*length < matchLength && matchLength >= minLength) {
            *length = matchLength;
            *distance = matchDistance;
        }
        // Si la longitud de match es mayor a la longitud de los datos del buffer
        // No se puede retroceder el cursor, es el match maximo
        if (cursorPos + matchLength > this->reader.bufferLength + this->reader.windowSize) return 1;
        // Retrocedo el cursor para buscar otro match
        this->reader.cursor -= matchLength;
    }
    // Vuelvo a avanzar el cursor para que los caracteres del match queden como ya leidos
    this->reader.cursor += *length;
    return 1;
}


int LzBufferedReader_readNextByte(LzBufferedReader* this, unsigned char* c) {
    // Si el cursor apunta al ultimo caracter leido del archivo
    if (this->reader.cursor == this->reader.bufferLength + this->reader.windowSize -1) {
        // Copio los ultimos caracteres del buffer en la ventana ya que en la
        // lectura del archivo seran sobreescritos
        size_t i;
        for (i=0; i < this->reader.windowSize; i++) {
            this->reader.buffer[i] = this->reader.buffer[this->reader.bufferLength + i];
        }
    }
    return BufferedReader_readNextByte(&(this->reader), c);
}

