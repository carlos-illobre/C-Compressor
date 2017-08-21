/* 
 * File:   BlockBufferedReader.h
 * Author: Carlos Illobre
 *
 * Lector de archivos que se asegura de leer el archivo hasta llenar todo el buffer
 *
 * Created on 10 de abril de 2011, 16:59
 */

#ifndef BLOCKBUFFEREDREADER_H
#define	BLOCKBUFFEREDREADER_H

#include "BufferedReader.h"


#ifdef	__cplusplus
extern "C" {
#endif


    typedef BufferedReader BlockBufferedReader;

    int BlockBufferedReader_open(BlockBufferedReader* this, const char* filename, size_t bufferSize, size_t windowSize);

    inline int BlockBufferedReader_isOpen(BlockBufferedReader* this);

    inline void BlockBufferedReader_close(BlockBufferedReader* this);

    /**
     * Carga en memoria el proximo bloque del archivo
     * @param this
     * @return true si el bloque pudo leerse y false en otro caso
     */
    int BlockBufferedReader_readNextBlock(BlockBufferedReader* this);

    inline int BlockBufferedReader_readNextSize_t(BlockBufferedReader* this, size_t* symbol);

    inline int BlockBufferedReader_readNextByte(BlockBufferedReader* this, unsigned char* c);


#ifdef	__cplusplus
}
#endif

#endif	/* BLOCKBUFFEREDREADER_H */

