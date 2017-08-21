/* 
 * File:   LzBufferedReader.h
 * Author: Carlos Illobre
 *
 * Created on 16 de abril de 2011, 13:51
 */

#ifndef LZBUFFEREDREADER_H
#define	LZBUFFEREDREADER_H

#include "BufferedReader.h"


#ifdef	__cplusplus
extern "C" {
#endif


    typedef struct LzBufferedReader {
        BufferedReader reader;
    } LzBufferedReader;


    int LzBufferedReader_open(LzBufferedReader* this, const char* filename, size_t bufferSize, size_t windowSize);
    void LzBufferedReader_close(LzBufferedReader* this);

    /**
     * Lee la siguiente posicion y distancia
     * @param this
     * @param length
     * @param distance
     * @return
     */
    int LzBufferedReader_readNextLengthDistance(LzBufferedReader* this, size_t* length, size_t* distance, size_t minLength);

    /**
     * Lee el siguiente caracter y lo deja en c
     * @param this
     * @param c
     * @return 0 si se llego a eof, 1 si se leyo el caracter sin recargar el buffer
     * y -1 si se leyo el caracter habiendo recargado el buffer
     */
    int LzBufferedReader_readNextByte(LzBufferedReader* this, unsigned char* c);


#ifdef	__cplusplus
}
#endif

#endif	/* LZBUFFEREDREADER_H */

