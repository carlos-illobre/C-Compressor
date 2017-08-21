/* 
 * File:   BufferedReader.h
 * Author: Carlos Illobre
 *
 * Created on 9 de abril de 2011, 17:07
 */

#ifndef BUFFEREDREADER_H
#define	BUFFEREDREADER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

    
    typedef struct BufferedReader {
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
        // Cantidad de bytes del buffer que se reservan y no son utilizados para
        // la lectura. Son los bytes iniciales del buffer
        size_t windowSize;
    } BufferedReader;

    
    /**
     * Sirve para que las subclases puedan manipular la primera lectura del buffer
     * @param this
     * @param filename
     * @param bufferSize
     * @param windowSize Cantidad de bytes iniciales del buffer que se reservan
     * y no seran utilizados para la lectura del archivo
     * @return
     */
    int BufferedReader_open(BufferedReader* this, const char* filename, size_t bufferSize, size_t windowSize);

    inline int BufferedReader_isOpen(BufferedReader* this);

    /**
     *
     * @param this
     * @return true si se llego al fin de archivo, false en otro caso
     */
    inline int BufferedReader_isEof(BufferedReader* this);

    /**
     * Cierra un AbstractBufferedReader liberando los recursos.
     * @param this El AbstractBufferedReader que se quiere cerrar
     */
    void BufferedReader_close(BufferedReader* this);

    /**
     * @param this El BufferedReader del que se quiere obtener el siguiente byte
     * @paran c Queda el byte leido del archivo
     * @return 1 si se pudo leer o 0 si se llego al fin de archivo.
     */
    int BufferedReader_readNextByte(BufferedReader* this, unsigned char* c);

    int BufferedReader_readNextSize_t(BufferedReader* this, size_t* symbol);
    

#ifdef	__cplusplus
}
#endif

#endif	/* BUFFEREDREADER_H */

