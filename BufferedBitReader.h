/* 
 * File:   BufferedBitReader.h
 * Author: Carlos Illobre
 *
 * Created on 10 de abril de 2011, 19:10
 */

#ifndef BUFFEREDBITREADER_H
#define	BUFFEREDBITREADER_H

#include <stdio.h>

#ifdef	__cplusplus
extern "C" {
#endif


    typedef struct BufferedBitReader {
        // Archivo que se esta leyendo
        FILE* file;
        // Buffer donde se guardan los bytes leidos del archivo
        unsigned char* buffer;
        // Tamaño fisico del buffer
        size_t bufferSize;
        // Tamaño del bloque de bytes que se leyo del archivo y se coloco en el buffer
        size_t bufferLength;
        // Posicion del proximo byte a leer
        size_t cursor;
        // Posicion del proximo bit a leer relativa al byte
        char bitCursor;
        // Cantidad de bits validos leidos del archivo y se colocados en la primera posicion del buffer
        char byteLength;
    } BufferedBitReader;


    int BufferedBitReader_open(BufferedBitReader* this, const char* filename, size_t bufferSize);

    inline int BufferedBitReader_isOpen(BufferedBitReader* this);

    /**
     * Cierra un BufferedBitReader liberando los recursos.
     * @param this El BufferedBitReader que se quiere cerrar
     */
    inline void BufferedBitReader_close(BufferedBitReader* this);

    /**
     *
     * @param this
     * @return El bit leido o -1 si se llego al fin de archivo
     */
    int BufferedBitReader_readBit(BufferedBitReader* this);

    /**
     *
     * @param this
     * @param c
     * @return La cantidad de bits leidos
     */
    int BufferedBitReader_read8Bit(BufferedBitReader* this, unsigned char* c);

    size_t BufferedBitReader_readUnary(BufferedBitReader* this);

    size_t BufferedBitReader_readPrefix(BufferedBitReader* this, size_t max);

    size_t BufferedBitReader_readGamma(BufferedBitReader* this);

    size_t BufferedBitReader_readDelta(BufferedBitReader* this);

#ifdef	__cplusplus
}
#endif

#endif	/* BUFFEREDBITREADER_H */

