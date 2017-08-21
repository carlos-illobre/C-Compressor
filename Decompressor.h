/* 
 * File:   Decompressor.h
 * Author: Carlos Illobre
 *
 * Created on 1 de junio de 2011, 23:38
 */

#ifndef DECOMPRESSOR_H
#define	DECOMPRESSOR_H

#include "BufferedBitReader.h"
#include "BufferedWriter.h"

#ifdef	__cplusplus
extern "C" {
#endif

    int Decompressor_adaptativeHuffman(const char* inputFileName, const char* outputFileName, size_t bufferSize);

    int Decompressor_lz77(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize);

    int Decompressor_lzss(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize);

    int Decompressor_lzhuff(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize);

    int Decompressor_blockSorting(const char* inputFileName, const char* outputFileName, size_t bufferSize);

    int Decompressor_moveToFront(const char* inputFileName, const char* outputFileName, size_t bufferSize);

    int Decompressor_blockSortingMoveToFront(const char* inputFileName, const char* outputFileName, size_t bufferSize);

    int Decompressor_blockSortingMoveToFrontAdaptativeHuffman(const char* inputFileName, const char* outputFileName, size_t bufferSize);

    int Decompressor_dynamicArithmetic(BufferedBitReader* reader, BufferedWriter* writer);

#ifdef	__cplusplus
}
#endif

#endif	/* DECOMPRESSOR_H */

