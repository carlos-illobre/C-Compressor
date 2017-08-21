#include "Decompressor.h"
#include "AdaptativeHuffman.h"
#include "DynamicArithmetic.h"
#include "ProgressMonitorBufferedReader.h"
#include "ProgressMonitorBlockBufferedReader.h"
#include "ErrorCodes.h"
#include "MoveToFront.h"
#include <limits.h>


int Decompressor_adaptativeHuffman(const char* inputFileName, const char* outputFileName, size_t bufferSize) {
    int errorCode=0;
    // Abro el archivo para lectura
    BufferedBitReader reader;
    // Si el archivo pudo abrirse
    if (BufferedBitReader_open(&reader, inputFileName, bufferSize)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize);
        // Si pudo abrirse
        if (writer) {
            AdaptativeHuffman huffman;
            AdaptativeHuffman_init(&huffman);
            // Leo cada simbolo hasta el fin de archivo
            Symbol c;
            while (AdaptativeHuffman_getSymbol_BufferedBitReader(&huffman, &c, &reader)) {
                // Escribo el caracter en el archivo de salido
                BufferedWriter_writeByte(writer, c);
                // Incremento la frecuencia del caracter en el arbol
                AdaptativeHuffman_incFreq(&huffman, c);
            }
            BufferedWriter_close(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        BufferedBitReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    return errorCode;
}


int Decompressor_blockSorting(const char* inputFileName, const char* outputFileName, size_t bufferSize) {
    int errorCode=0;
    // Abro el archivo para lectura
    ProgressMonitorBlockBufferedReader reader;
    // Si el archivo pudo abrirse
    if (ProgressMonitorBlockBufferedReader_open(&reader, inputFileName, bufferSize + sizeof(size_t), 0)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize);
        // Si pudo abrirse
        if (writer) {
            unsigned char c;
            do {
                // Leo el indice
                size_t index;
                if (!ProgressMonitorBlockBufferedReader_readNextSize_t(&reader, &index)) break;
                // Decodifico
                BlockSorting_decode(reader.reader.buffer+sizeof(size_t), index, reader.reader.bufferLength-sizeof(size_t));
                // Escribo lo decodificado
                while (ProgressMonitorBlockBufferedReader_readNextByte(&reader, &c)) {
                    BufferedWriter_writeByte(writer, c);
                }
            } while(ProgressMonitorBlockBufferedReader_readNextBlock(&reader));
            BufferedWriter_close(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        ProgressMonitorBlockBufferedReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    return errorCode;
}


int Decompressor_blockSortingMoveToFront(const char* inputFileName, const char* outputFileName, size_t bufferSize) {
    int errorCode=0;
    // Abro el archivo para lectura
    ProgressMonitorBlockBufferedReader reader;
    // Si el archivo pudo abrirse
    if (ProgressMonitorBlockBufferedReader_open(&reader, inputFileName, bufferSize + sizeof(size_t), 0)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize);
        // Si pudo abrirse
        if (writer) {
            MoveToFront mtf;
            if (MoveToFront_create(&mtf)) {
                unsigned char c;
                do {
                    // Leo el indice
                    size_t index;
                    if (!ProgressMonitorBlockBufferedReader_readNextSize_t(&reader, &index)) break;
                    // Decodifico
                    MoveToFront_decode(&mtf, reader.reader.buffer+sizeof(size_t), reader.reader.bufferLength-sizeof(size_t));
                    BlockSorting_decode(reader.reader.buffer+sizeof(size_t), index, reader.reader.bufferLength-sizeof(size_t));
                    // Escribo lo decodificado
                    while (ProgressMonitorBlockBufferedReader_readNextByte(&reader, &c)) {
                        BufferedWriter_writeByte(writer, c);
                    }
                } while(ProgressMonitorBlockBufferedReader_readNextBlock(&reader));
                MoveToFront_destroy(&mtf);
            } else errorCode = ERROR_CODE_NOT_FOUND;
            BufferedWriter_close(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        ProgressMonitorBlockBufferedReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    return errorCode;
}


int Decompressor_moveToFront(const char* inputFileName, const char* outputFileName, size_t bufferSize) {
    int errorCode=0;
    // Abro el archivo para lectura
    ProgressMonitorBlockBufferedReader reader;
    // Si el archivo pudo abrirse
    if (ProgressMonitorBlockBufferedReader_open(&reader, inputFileName, bufferSize, 0)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize);
        // Si pudo abrirse
        if (writer) {
            MoveToFront mtf;
            if (MoveToFront_create(&mtf)) {
                do {
                    MoveToFront_decode(&mtf, reader.reader.buffer, reader.reader.bufferLength);
                    int i;
                    for (i=0; i<reader.reader.bufferLength; i++) BufferedWriter_writeByte(writer, reader.reader.buffer[i]);
                } while(ProgressMonitorBlockBufferedReader_readNextBlock(&reader));
                MoveToFront_destroy(&mtf);
            } else errorCode = ERROR_CODE_NOT_FOUND;
            BufferedWriter_close(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        ProgressMonitorBlockBufferedReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    return errorCode;
}


int Decompressor_blockSortingMoveToFrontAdaptativeHuffman(const char* inputFileName, const char* outputFileName, size_t bufferSize) {
    unsigned char* block = (unsigned char*)malloc(bufferSize*sizeof(unsigned char));
    if (!block) return ERROR_CODE_OUT_OF_MEMORY;
    int errorCode=0;
    // Abro el archivo para lectura
    BufferedBitReader reader;
    // Si el archivo pudo abrirse
    if (BufferedBitReader_open(&reader, inputFileName, bufferSize/2)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize/2);
        // Si pudo abrirse
        if (writer) {
            MoveToFront mtf;
            AdaptativeHuffman huffman;
            AdaptativeHuffman_init(&huffman);
            if (MoveToFront_create(&mtf)) {
                size_t index;
                unsigned char* indexAux = (unsigned char*)&index;
                size_t blockCursor = 0;
                int j = 1;
                Symbol symbol;
                while (AdaptativeHuffman_getSymbol_BufferedBitReader(&huffman, &symbol, &reader)) {
                    // Si se esta leyendo el indice
                    if (j<=sizeof(size_t)) {
                        // Armo el indice
                        indexAux[sizeof(size_t)-j] = (unsigned char)symbol;
                        j++;
                    // Si se termino de leer el indice
                    } else {
                        // Si el bloque no esta lleno
                        if (blockCursor < bufferSize) {
                            // Lleno el buffer
                            block[blockCursor] = (unsigned char)symbol;
                            blockCursor++;
                            // Si el bloque esta lleno
                            if (blockCursor == bufferSize) {
                                // Decodifico
                                MoveToFront_decode(&mtf, block, bufferSize);
                                BlockSorting_decode(block, index, bufferSize);
                                // Escribo lo decodificado
                                BufferedWriter_writeByteArray(writer, block, bufferSize);
                                // Indico que hay que leer un indice
                                j = 1;
                                blockCursor = 0;
                            }
                        }
                    }
                    // Incremento la frecuencia del caracter en el arbol
                    AdaptativeHuffman_incFreq(&huffman, symbol);
                }
                // Se procesa el bloque final
                if (blockCursor) {
                    // Decodifico
                    MoveToFront_decode(&mtf, block, blockCursor);
                    BlockSorting_decode(block, index, blockCursor);
                    // Escribo lo decodificado
                    BufferedWriter_writeByteArray(writer, block, blockCursor);
                }
                MoveToFront_destroy(&mtf);
            } else errorCode = ERROR_CODE_NOT_FOUND;
            BufferedWriter_close(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        BufferedBitReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    free(block);
    return errorCode;
}


int Decompressor_lz77(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize) {
    int errorCode = 0;
    // Abro el archivo para lectura
    ProgressMonitorBufferedReader reader;
    // Si el archivo pudo abrirse
    if (ProgressMonitorBufferedReader_open(&reader, inputFileName, bufferSize)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize);
        // Si pudo abrirse
        if (writer) {
            unsigned char c;
            size_t length, distance, i, windowPos;
            char* window = (char*)malloc(windowSize);
            size_t windowCursor = 0;
            // Se lee una longitud
            while (ProgressMonitorBufferedReader_readByte(&reader, &c)) {
                // Si la longitud es UCHAR_MAX sigo leyendo longitudes hasta que no sea UCHAR_MAX (no puede ser eof)
                for (length=0; c == UCHAR_MAX; length += UCHAR_MAX) ProgressMonitorBufferedReader_readByte(&reader, &c);
                // Se suma la ultima longitud que no es UCHAR_MAX
                length += c;
                // Si la longitud no es cero lo que sigue es una posicion
                if (length) {
                    // Leo la posicion, es UCHAR_MAX sigo leyendo longitudes hasta que no sea UCHAR_MAX (no puede ser eof)
                    for (distance=0; ProgressMonitorBufferedReader_readByte(&reader, &c) && c == UCHAR_MAX; distance += UCHAR_MAX);
                    // Se suma la ultima posicion que no es UCHAR_MAX
                    distance += c;
                    // Escribo los caracteres que hay en la memoria segun la longitud y distancia
                    windowPos = windowCursor > distance+1 ? windowCursor-1-distance : windowSize+windowCursor-1-distance;
                    for (i=0; i<length; i++) {
                        window[windowCursor] = window[windowPos + i < windowSize ? windowPos+i : windowPos+i-windowSize];
                        BufferedWriter_writeByte(writer, window[windowCursor]);
                        windowCursor++;
                        if (windowCursor == windowSize) windowCursor = 0;
                    }
                }
                // Leo el caracter colocandolo en la memoria
                // Si no se llego al eof
                if (ProgressMonitorBufferedReader_readByte(&reader, window + windowCursor)) {
                    // Escribo el caracter
                    BufferedWriter_writeByte(writer, window[windowCursor]);
                    windowCursor++;
                    if (windowCursor == windowSize) windowCursor = 0;
                }
            }
            free(window);
            BufferedWriter_close(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        ProgressMonitorBufferedReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    return errorCode;
}


int Decompressor_lzss(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize) {
    int errorCode = 0;
    // Abro el archivo para lectura
    BufferedBitReader reader;
    // Si el archivo pudo abrirse
    if (BufferedBitReader_open(&reader, inputFileName, bufferSize)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize);
        // Si pudo abrirse
        if (writer) {
            unsigned char c;
            size_t length, distance, i, windowPos;
            int bit = 0;
            char* window = (char*)malloc(windowSize);
            size_t windowCursor = 0;
            while (bit != -1) {
                bit = BufferedBitReader_readBit(&reader);
                switch (bit) {
                    // Si el bit es un uno hay que leer una longitud
                    case 1:
                        // Leo la longitud
                        length = 0;
                        for (BufferedBitReader_read8Bit(&reader, &c); c == UCHAR_MAX; BufferedBitReader_read8Bit(&reader, &c)) {
                            length += UCHAR_MAX;
                        }
                        length += c;
                        // leo la posicion
                        distance = 0;
                        for (BufferedBitReader_read8Bit(&reader, &c); c == UCHAR_MAX; BufferedBitReader_read8Bit(&reader, &c)) {
                            distance += UCHAR_MAX;
                        }
                        distance += c;
                        // Escribo los caracteres que hay en la memoria segun la longitud y distancia
                        windowPos = windowCursor > distance+1 ? windowCursor-1-distance : windowSize+windowCursor-1-distance;
                        for (i=0; i<length; i++) {
                            window[windowCursor] = window[windowPos + i < windowSize ? windowPos+i : windowPos+i-windowSize];
                            BufferedWriter_writeByte(writer, window[windowCursor]);
                            windowCursor++;
                            if (windowCursor == windowSize) windowCursor = 0;
                        }
                        break;
                    // Si el bit es cero hay que leer un caracter
                    case 0:
                        // Se lee el caracter
                        BufferedBitReader_read8Bit(&reader, window + windowCursor);
                        // Escribo el caracter
                        BufferedWriter_writeByte(writer, window[windowCursor]);
                        windowCursor++;
                        if (windowCursor == windowSize) windowCursor = 0;
                        break;
                }
            }
            free(window);
            BufferedWriter_close(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        LzBufferedReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    return errorCode;
}


int Decompressor_lzhuff(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize) {
    int errorCode = 0;
    // Abro el archivo para lectura
    BufferedBitReader reader;
    // Si el archivo pudo abrirse
    if (BufferedBitReader_open(&reader, inputFileName, bufferSize)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize);
        // Si pudo abrirse
        if (writer) {
            AdaptativeHuffman huffman;
            AdaptativeHuffman_init(&huffman);
            // Leo cada simbolo hasta el fin de archivo
            Symbol c;
            size_t length, distance, i, windowPos;
            unsigned char* window = (unsigned char*)malloc(windowSize);
            size_t windowCursor = 0;
            // Mientras se pueda leer un simbolo
            while (AdaptativeHuffman_getSymbol_BufferedBitReader(&huffman, &c, &reader)) {
                // Incremento la frecuencia del simbolo en el arbol
                AdaptativeHuffman_incFreq(&huffman, c);
                // Si el simbolo es un caracter
                if (c < UCHAR_MAX) {
                    window[windowCursor] = c;
                    // Escribo el caracter
                    BufferedWriter_writeByte(writer, window[windowCursor]);
                    windowCursor++;
                    if (windowCursor == windowSize) windowCursor = 0;
                // Si el simbolo no es un caracter entonces es una longitud
                } else {
                    // Mientras la longitud leida sea la maxima
                    for (length=0; c == HUFFMAN_MAX_LENGTH + UCHAR_MAX;) {
                        length += HUFFMAN_MAX_LENGTH;
                        // Leo otra longitud
                        AdaptativeHuffman_getSymbol_BufferedBitReader(&huffman, &c, &reader);
                        // Incremento la frecuencia del simbolo en el arbol
                        AdaptativeHuffman_incFreq(&huffman, c);
                    }
                    length += c-UCHAR_MAX;
                    // leo la posicion
                    distance = BufferedBitReader_readPrefix(&reader, windowSize-1);
                    // Escribo los caracteres que hay en la memoria segun la longitud y distancia
                    windowPos = windowCursor > distance+1 ? windowCursor-1-distance : windowSize+windowCursor-1-distance;
                    for (i=0; i<length; i++) {
                        window[windowCursor] = window[windowPos + i < windowSize ? windowPos+i : windowPos+i-windowSize];
                        BufferedWriter_writeByte(writer, window[windowCursor]);
                        windowCursor++;
                        if (windowCursor == windowSize) windowCursor = 0;
                    }
                }
            }
            free(window);
            BufferedWriter_close(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        LzBufferedReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    return errorCode;
}


int Decompressor_dynamicArithmetic(BufferedBitReader* reader, BufferedWriter* writer) {
    DynamicArithmetic compressor;
    DynamicArithmetic_create(&compressor, UCHAR_MAX);
    // Leo cada caracter hasta el fin de archivo
    unsigned int c;
    while (DynamicArithmetic_decompress(&compressor, &c, reader)) {
        // Escribo el caracter en el archivo de salido
        BufferedWriter_writeByte(writer, c);
    }
    return 0;
}