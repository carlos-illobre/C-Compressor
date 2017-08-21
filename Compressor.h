/* 
 * File:   Compressor.h
 * Author: Carlos Illobre
 *
 * Created on 25 de marzo de 2011, 04:15
 */

#ifndef COMPRESSOR_H
#define	COMPRESSOR_H


#ifdef	__cplusplus
extern "C" {
#endif

#include "BufferedReader.h"
#include "ProgressMonitorBufferedReader.h"
#include "BufferedWriter.h"
#include "ProgressMonitor.h"
#include "BufferedBitReader.h"
#include "ErrorCodes.h"


    /**
     * Copia el archivo en entrada en el archivo de salida sin comprimirlo.
     * Sirve para comparar la velociada de compresion al restarle la velocidad
     * de lectura de disco
     * @param bufferedReader
     * @param bufferedWriter
     * @return 
     */
    int Compressor_none(const char* inputFileName, const char* outputFileName, size_t bufferSize);

    /**
     * Comprime utilizando el metodo de huffman dinamico
     * @param bufferedReader
     * @param bufferedWriter
     * @return ERROR_CODE_DYNAMIC_HUFFMAN en caso de error o cero en caso contrario
     */
    int Compressor_adaptiveHuffman(const char* inputFileName, const char* outputFileName, size_t bufferSize);

    int Compressor_moveToFront(const char* inputFileName, const char* outputFileName, size_t bufferSize);

    int Compressor_blockSorting(const char* inputFileName, const char* outputFileName, size_t bufferSize);

    int Compressor_blockSortingMoveToFront(const char* inputFileName, const char* outputFileName, size_t bufferSize);

    int Compressor_blockSortingMoveToFrontBlockSorting(const char* inputFileName, const char* outputFileName, size_t bufferSize);

    int Compressor_blockSortingMoveToFrontAdaptativeHuffman(const char* inputFileName, const char* outputFileName, size_t bufferSize);

    int Compressor_lz77(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize);

    int Compressor_lzss(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize);

    int Compressor_lzhuff(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize);

    int Compressor_blockSortingMoveToFrontBlockSortingLzhuff(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize);
    
    int Compressor_dynamicArithmetic(BufferedReader* bufferedReader, BufferedWriter* bufferedWriter);

    /**
     * Comprime un archivo colocando el contenido comprimido en otro archivo
     * @param inputFileName
     */
    int Compressor_process(const char* inputFileName, const char* outputFileName, int (*compressDecompress)(ProgressMonitorBufferedReader*, BufferedWriter*));


#ifdef	__cplusplus
}
#endif

#endif	/* COMPRESSOR_H */

