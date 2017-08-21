#include <limits.h>
#include "Compressor.h"
#include "ProgressMonitorBufferedReader.h"
#include "ProgressMonitorBlockBufferedReader.h"
#include "AdaptativeHuffman.h"
#include "DynamicArithmetic.h"
#include "BlockSorting.h"
#include "BufferedBitReader.h"
#include "MoveToFront.h"
#include "LzBufferedReader.h"


int Compressor_none(const char* inputFileName, const char* outputFileName, size_t bufferSize) {
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
            while (ProgressMonitorBufferedReader_readByte(&reader, &c)) {
                BufferedWriter_writeByte(writer, c);
            }
            BufferedWriter_close(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        ProgressMonitorBufferedReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    return errorCode;
}


int Compressor_lz77(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize) {
    int errorCode = 0;
    // Abro el archivo para lectura
    LzBufferedReader reader;
    // Si el archivo pudo abrirse
    if (LzBufferedReader_open(&reader, inputFileName, bufferSize, windowSize)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize);
        // Si pudo abrirse
        if (writer) {
            unsigned char c;
            size_t length, distance;
            while (LzBufferedReader_readNextLengthDistance(&reader, &length, &distance, 1)) {
                // Se guarda la longitud
                int lengthSaved = 0;
                for (; length >= UCHAR_MAX; length -= UCHAR_MAX) {
                    // Para longitudes mayores a UCHAR_MAX escribo tantos UCHAR_MAX como hagan falta
                    BufferedWriter_writeByte(writer, UCHAR_MAX);
                    lengthSaved = 1;
                }
                // Escribo la longitud menor a UCHAR_MAX (si es cero marca el final de la secuencia de UCHAR_MAX)
                BufferedWriter_writeByte(writer, length);
                if (length || lengthSaved) {
                    // Se guarda la posicion
                    for (; distance >= UCHAR_MAX; distance -= UCHAR_MAX) {
                        // Para distancias mayores a UCHAR_MAX escribo tantos UCHAR_MAX como hagan falta
                        BufferedWriter_writeByte(writer, UCHAR_MAX);
                    }
                    // Escribo la distancia menor a UCHAR_MAX (si es cero marca el final de la secuencia de UCHAR_MAX)
                    BufferedWriter_writeByte(writer, distance);
                    lengthSaved = 0;
                }
                if (LzBufferedReader_readNextByte(&reader, &c)) BufferedWriter_writeByte(writer, c);
            }
            BufferedWriter_close(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        LzBufferedReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    return errorCode;
}


int Compressor_lzss(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize) {
    int errorCode = 0;
    // Abro el archivo para lectura
    LzBufferedReader reader;
    // Si el archivo pudo abrirse
    if (LzBufferedReader_open(&reader, inputFileName, bufferSize, windowSize)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize);
        // Si pudo abrirse
        if (writer) {
            unsigned char c;
            size_t length, distance;
            while (LzBufferedReader_readNextLengthDistance(&reader, &length, &distance, 2)) {
                // Si la longitud es distinta de cero
                if (length) {
                    // Se guarda un bit 1
                    BufferedWriter_writeBit(writer, 1);
                    // Se guarda la longitud
                    for (; length >= UCHAR_MAX; length -= UCHAR_MAX) {
                        // Para longitudes mayores a 255 escribo tantos 255 como hagan falta
                        BufferedWriter_write8Bit(writer, UCHAR_MAX);
                    }
                    // Escribo la longitud menor a 255 (si es cero marca el final de la secuencia de 255)
                    BufferedWriter_write8Bit(writer, length);
                    // Se guarda la posicion
                    for (; distance >= UCHAR_MAX; distance -= UCHAR_MAX) {
                        // Para distancias mayores a 255 escribo tantos 255 como hagan falta
                        BufferedWriter_write8Bit(writer, UCHAR_MAX);
                    }
                    // Escribo la distancia menor a 255 (si es cero marca el final de la secuencia de 255)
                    BufferedWriter_write8Bit(writer, distance);
                // Si la posicion es igual a cero
                } else {
                    // Se lee el caracter
                    LzBufferedReader_readNextByte(&reader, &c);
                    // Se guarda un bit 0
                    BufferedWriter_writeBit(writer, 0);
                    // Se guarda el caracter
                    BufferedWriter_write8Bit(writer, c);
                }
            }
            BufferedWriter_closeWithEof(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        LzBufferedReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    return errorCode;
}


int Compressor_adaptiveHuffman(const char* inputFileName, const char* outputFileName, size_t bufferSize) {
    int errorCode=0;
    // Abro el archivo para lectura
    ProgressMonitorBufferedReader reader;
    // Si el archivo pudo abrirse
    if (ProgressMonitorBufferedReader_open(&reader, inputFileName, bufferSize)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize);
        // Si pudo abrirse
        if (writer) {
            AdaptativeHuffman huffman;
            AdaptativeHuffman_init(&huffman);
            // Leo cada caracter hasta el fin de archivo
            unsigned char c;
            while (ProgressMonitorBufferedReader_readByte(&reader, &c)) {
                // Leo el codigo del caracter en el arbol de huffman y lo escribo en el archivo
                if (AdaptativeHuffman_getCode_BufferedWriter(&huffman, c, writer)) {
                    // Incremento la frecuencia del caracter en el arbol
                    AdaptativeHuffman_incFreq(&huffman, c);
                } else errorCode = ERROR_CODE_NOT_FOUND;
            }
            BufferedWriter_closeWithEof(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        ProgressMonitorBufferedReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    return errorCode;
}


int Compressor_lzhuff(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize) {
    int errorCode=0;
    // Abro el archivo para lectura
    LzBufferedReader reader;
    // Si el archivo pudo abrirse
    if (LzBufferedReader_open(&reader, inputFileName, bufferSize, windowSize)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize);
        // Si pudo abrirse
        if (writer) {
            AdaptativeHuffman huffman;
            AdaptativeHuffman_init(&huffman);
            // Leo cada caracter hasta el fin de archivo
            unsigned char c;
            size_t length, distance;
            while (!errorCode && LzBufferedReader_readNextLengthDistance(&reader, &length, &distance, 1)) {
                // Si la longitud es distinta de cero
                if (length) {
                    // Se guarda la longitud
                    // Para longitudes mayores a la maxima escribo tantas longitudes maximas como hagan falta
                    for (; length >= HUFFMAN_MAX_LENGTH; length -= HUFFMAN_MAX_LENGTH) {
                        // Leo el codigo del caracter en el arbol de huffman y lo escribo en el archivo
                        if (AdaptativeHuffman_getCode_BufferedWriter(&huffman, HUFFMAN_MAX_LENGTH + UCHAR_MAX, writer)) {
                            // Incremento la frecuencia del caracter en el arbol
                            AdaptativeHuffman_incFreq(&huffman, HUFFMAN_MAX_LENGTH + UCHAR_MAX);
                        } else errorCode = ERROR_CODE_NOT_FOUND;
                    }
                    // Escribo la ultima longitud (si es cero marca el final de la secuencia de longitudes maximas)
                    if (AdaptativeHuffman_getCode_BufferedWriter(&huffman, length + UCHAR_MAX, writer)) {
                        // Incremento la frecuencia del caracter en el arbol
                        AdaptativeHuffman_incFreq(&huffman, length + UCHAR_MAX);
                    } else errorCode = ERROR_CODE_NOT_FOUND;
                    // Se guarda la distancia
                    BufferedWriter_writePrefix(writer, distance, windowSize-1);
                // Si la longitud es igual a cero
                } else {
                    // Se lee el caracter
                    LzBufferedReader_readNextByte(&reader, &c);
                    // Leo el codigo del caracter en el arbol de huffman y lo escribo en el archivo
                    if (AdaptativeHuffman_getCode_BufferedWriter(&huffman, c, writer)) {
                        // Incremento la frecuencia del caracter en el arbol
                        AdaptativeHuffman_incFreq(&huffman, c);
                    } else errorCode = ERROR_CODE_NOT_FOUND;
                }
            }
            BufferedWriter_closeWithEof(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        LzBufferedReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    return errorCode;
}


int Compressor_blockSortingMoveToFrontBlockSortingLzhuff(const char* inputFileName, const char* outputFileName, size_t bufferSize, size_t windowSize) {
    int errorCode=0;
    return errorCode;
}


int Compressor_moveToFront(const char* inputFileName, const char* outputFileName, size_t bufferSize) {
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
                    MoveToFront_code(&mtf, reader.reader.buffer, reader.reader.bufferLength);
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


int Compressor_blockSorting(const char* inputFileName, const char* outputFileName, size_t bufferSize) {
    int errorCode=0;
    // Abro el archivo para lectura
    ProgressMonitorBlockBufferedReader reader;
    // Si el archivo pudo abrirse
    if (ProgressMonitorBlockBufferedReader_open(&reader, inputFileName, bufferSize, 0)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize);
        // Si pudo abrirse
        if (writer) {
            unsigned char c;
            do {
                // Se codifica el bloque obteniendose el indice de la fila original en la matriz
                size_t index = BlockSorting_code(reader.reader.buffer, reader.reader.bufferLength);
                // Escribo el indice
                BufferedWriter_writeSize_t(writer, index);
                // Escribo el bloque en el archivo
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


int Compressor_blockSortingMoveToFront(const char* inputFileName, const char* outputFileName, size_t bufferSize) {
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
                unsigned char c;
                do {
                    size_t index = BlockSorting_code(reader.reader.buffer, reader.reader.bufferLength);
                    MoveToFront_code(&mtf, reader.reader.buffer, reader.reader.bufferLength);
                    // Escribo el indice
                    BufferedWriter_writeSize_t(writer, index);
                    // Escribo el bloque en el archivo
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


int Compressor_blockSortingMoveToFrontBlockSorting(const char* inputFileName, const char* outputFileName, size_t bufferSize) {
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
                    BlockSorting_code(reader.reader.buffer, reader.reader.bufferLength);
                    MoveToFront_code(&mtf, reader.reader.buffer, reader.reader.bufferLength);
                    BlockSorting_code(reader.reader.buffer, reader.reader.bufferLength);
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


int Compressor_blockSortingMoveToFrontAdaptativeHuffman(const char* inputFileName, const char* outputFileName, size_t bufferSize) {
    int errorCode=0;
    // Abro el archivo para lectura
    ProgressMonitorBlockBufferedReader reader;
    // Si el archivo pudo abrirse
    if (ProgressMonitorBlockBufferedReader_open(&reader, inputFileName, bufferSize, 0)) {
        // Abro el archivo de salida
        BufferedWriter* writer = BufferedWriter_open(outputFileName, bufferSize);
        // Si pudo abrirse
        if (writer) {
            AdaptativeHuffman huffman;
            AdaptativeHuffman_init(&huffman);
            MoveToFront mtf;
            if (MoveToFront_create(&mtf)) {
                unsigned char c;
                do {
                    size_t index = BlockSorting_code(reader.reader.buffer, reader.reader.bufferLength);
                    MoveToFront_code(&mtf, reader.reader.buffer, reader.reader.bufferLength);
                    int i;
                    // Comprimo el indice
                    unsigned char* aux = (unsigned char*)(&index);
                    for (i=1; !errorCode && i<=sizeof(size_t); i++) {
                        c = aux[sizeof(size_t)-i];
                        // Leo el codigo del caracter en el arbol de huffman y lo escribo en el archivo
                        if (AdaptativeHuffman_getCode_BufferedWriter(&huffman, c, writer)) {
                            // Incremento la frecuencia del caracter en el arbol
                            AdaptativeHuffman_incFreq(&huffman, c);
                        } else {
                            errorCode = ERROR_CODE_NOT_FOUND;
                        }
                    }
                    // Comprimo el bloque
                    while (!errorCode && ProgressMonitorBlockBufferedReader_readNextByte(&reader, &c)) {
                        // Leo el codigo del caracter en el arbol de huffman y lo escribo en el archivo
                        if (AdaptativeHuffman_getCode_BufferedWriter(&huffman, c, writer)) {
                            // Incremento la frecuencia del caracter en el arbol
                            AdaptativeHuffman_incFreq(&huffman, c);
                        } else {
                            errorCode = ERROR_CODE_NOT_FOUND;
                        }
                    }
                } while(!errorCode && ProgressMonitorBlockBufferedReader_readNextBlock(&reader));
                MoveToFront_destroy(&mtf);
            } else errorCode = ERROR_CODE_NOT_FOUND;
            BufferedWriter_closeWithEof(writer);
        } else errorCode = ERROR_CODE_CANT_CREATE_OUTPUT_FILE;
        ProgressMonitorBlockBufferedReader_close(&reader);
    } else errorCode = ERROR_CODE_INPUT_FILE_NOT_FOUND;
    return errorCode;
}


int Compressor_dynamicArithmetic(BufferedReader* bufferedReader, BufferedWriter* bufferedWriter) {
    DynamicArithmetic compressor;
    DynamicArithmetic_create(&compressor, UCHAR_MAX);
    // Leo cada caracter hasta el fin de archivo
    unsigned char c;
    while (BufferedReader_readNextByte(bufferedReader, &c)) {
        // Comprimo el caracter lo escribo en el archivo
        DynamicArithmetic_compress(&compressor, c, bufferedWriter);
    }
    return 0;
}




