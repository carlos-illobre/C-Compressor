/* 
 * File:   BufferedWriter.h
 * Author: Carlos Illobre
 *
 * Created on 25 de marzo de 2011, 00:23
 */

#ifndef BUFFEREDWRITER_H
#define	BUFFEREDWRITER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "ArrayBit.h"

    typedef struct {
        // Archivo que se esta escribiendo
        FILE* file;
        // Buffer donde se guardan los bytes a escribir en el archivo
        unsigned char* buffer;
        // Tamaño fisico del buffer
        size_t bufferSize;
        // Posicion en el buffer donde se puede colocar el proximo byte
        size_t cursor;
        // Posicion en el buffer donde se puede colocar el proximo bit relativo al byte
        char bitCursor;
    } BufferedWriter;

    /**
     * Crea un buffer para escribir un archivo
     * @param filename Nombre del archivo que se quiere escribir
     * @param bufferSize Tamaño en memoria que tendra el buffer
     * @return Un puntero a un BufferedWriter listo para usar o NULL si no hay
     * suficiente memoria para crear el buffer o no se puede crear el archivo.
     */
    BufferedWriter* BufferedWriter_open(const char* filename, size_t bufferSize);

    /**
     * Cierra un BufferedWriter liberando los recursos.
     * @param this El BufferedWriter que se quiere cerrar
     */
    void BufferedWriter_close(BufferedWriter* this);

    /**
     * @param this El BufferedWriter donde se va a escribir el byte
     * @paran c Byte que se quiere escribir en el archivo
     */
    void BufferedWriter_writeByte(BufferedWriter* this, unsigned char c);

    void BufferedWriter_writeByteArray(BufferedWriter* this, unsigned char* array, size_t n);

    void BufferedWriter_writeSize_t(BufferedWriter* this, size_t symbol);

    /**
     * @param this El BufferedWriter donde se va a escribir el bit
     * @paran bit Bit que se quiere escribir en el archivo
     */
    void BufferedWriter_writeBit(BufferedWriter* this, int bit);

    /**
     * @param this El BufferedWriter donde se va a escribir el bit
     * @paran bits Bit que se quieren escribir en el archivo
     */
    void BufferedWriter_write8Bit(BufferedWriter* this, unsigned char bits);

    /**
     * Escribe el valor de n en codigo unario. El valor de n debe ser mayor que cero
     * @param this
     * @param n
     */
    void BufferedWriter_writeUnary(BufferedWriter* this, size_t n);

    void BufferedWriter_writeBinary(BufferedWriter* this, size_t n);

    /**
     * Escribe el valor de n en codigo prefijo. Admite n con valor cero
     * @param this
     * @param n
     * @param max Maximo valor posible de n
     */
    void BufferedWriter_writePrefix(BufferedWriter* this, size_t n, size_t max);

    void BufferedWriter_writeGamma(BufferedWriter* this, size_t n);

    void BufferedWriter_writeDelta(BufferedWriter* this, size_t n);

    /**
     * Cierra el archivo agregando una marca de EOF
     * @param this
     */
    void BufferedWriter_closeWithEof(BufferedWriter* this);


#ifdef	__cplusplus
}
#endif

#endif	/* BUFFEREDWRITER_H */

