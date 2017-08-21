/*
 * File:   main.c
 * Author: Carlos Illobre
 *
 * Created on 11 de marzo de 2011, 22:29
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "ArgumentValidator.h"
#include "Compressor.h"
#include "Decompressor.h"


#define FILE_OUTPUT_EXTENSION ".XX"

#define ERROR_MSG_INPUT_FILE_NOT_FOUND "No se encuentra el archivo"
#define ERROR_MSG_CANT_CREATE_OUTPUT_FILE "No se puede crear el archivo de salida"
#define ERROR_MSG_CODE_NOT_FOUND "El ultimo caracter leido tiene frecuencia cero"
#define ERROR_MSG_NO_COMPRESS_INPUT_FILE "Debe ingresar el nombre del archivo a comprimir.\n"
#define ERROR_MSG_NO_DECOMPRESS_INPUT_FILE "Debe ingresar el nombre del archivo a descomprimir.\n"
#define ERROR_MSG_INVALID_ARGUMENTS "Argumentos inválidos. Ejecute el comando sin argumentos para ver la ayuda.\n"
#define ERROR_MSG_DECOMPRESSOR_NOT_IMPLEMENTED "Descompressor no implementado."
#define ERROR_MSG_OUT_OF_MEMORY "Memoria insuficiente"
#define HELP_MESSAGE "Funcionamiento por línea de comandos:\n\nTPGrupoXX " ARG_COMPR " pepe.txt\n\nComprime el achivo pepe.txt generando el archivo pepe.txt.XX\n\nTPGrupoXX " ARG_DESC " pepe.txt.XX\n\nDescomprime el archivo pepe.XX re-creando el archivo original (pepe.txt)\n"


int process(const char* inputFileName, const char* outputFileName, int algoritm, int compress, size_t bufferSize, size_t windowSize) {
    switch (algoritm) {
        case ALG_NONE:
            printf("Sin compresion\n");
            return Compressor_none(inputFileName, outputFileName, bufferSize);
        case ALG_DHUFFMAN:
            printf("Huffman Adaptativo\n");
            return compress ? Compressor_adaptiveHuffman(inputFileName, outputFileName, bufferSize) : Decompressor_adaptativeHuffman(inputFileName, outputFileName, bufferSize);
        case ALG_MTF:
            printf("MoveToFront\n");
            return compress ? Compressor_moveToFront(inputFileName, outputFileName, bufferSize) : Decompressor_moveToFront(inputFileName, outputFileName, bufferSize);
        case ALG_BS:
            printf("BlockSorting\n");
            return compress ? Compressor_blockSorting(inputFileName, outputFileName, bufferSize) : Decompressor_blockSorting(inputFileName, outputFileName, bufferSize);
        case ALG_BSMTF:
            printf("BlockSorting + MoveToFront\n");
            return compress ? Compressor_blockSortingMoveToFront(inputFileName, outputFileName, bufferSize) : Decompressor_blockSortingMoveToFront(inputFileName, outputFileName, bufferSize);
        case ALG_LZ77:
            printf("Lz77\n");
            return compress ? Compressor_lz77(inputFileName, outputFileName, bufferSize, windowSize) : Decompressor_lz77(inputFileName, outputFileName, bufferSize, windowSize);
        case ALG_BSMTFBS:
            printf("BlockSorting + MoveToFront + BlockSorting\n");
            return compress ? Compressor_blockSortingMoveToFrontBlockSorting(inputFileName, outputFileName, bufferSize) : ERROR_CODE_DECOMPRESSOR_NOT_IMPLEMENTED;
        case ALG_BSMTF_DHUFFMAN:
            printf("BlockSorting + MoveToFront + Huffman Adaptativo\n");
            return compress ? Compressor_blockSortingMoveToFrontAdaptativeHuffman(inputFileName, outputFileName, bufferSize) : Decompressor_blockSortingMoveToFrontAdaptativeHuffman(inputFileName, outputFileName, bufferSize);
        case ALG_LZSS:
            printf("Lzss\n");
            return compress ? Compressor_lzss(inputFileName, outputFileName, bufferSize, windowSize) : Decompressor_lzss(inputFileName, outputFileName, bufferSize, windowSize);
        case ALG_LZHUFF:
            printf("Lzhuff\n");
            return compress ? Compressor_lzhuff(inputFileName, outputFileName, bufferSize, windowSize) : Decompressor_lzhuff(inputFileName, outputFileName, bufferSize, windowSize);
        default:
            printf("BlockSorting + MoveToFront + Huffman Adaptativo\n");
            return compress ? Compressor_blockSortingMoveToFrontAdaptativeHuffman(inputFileName, outputFileName, bufferSize) : Decompressor_blockSortingMoveToFrontAdaptativeHuffman(inputFileName, outputFileName, bufferSize);
/*
            printf("BlockSorting + MoveToFront + BlockSorting + Lzhuff\n");
            return compress ? Compressor_blockSortingMoveToFrontBlockSortingLzhuff(inputFileName, outputFileName, bufferSize, windowSize) : ERROR_CODE_DECOMPRESSOR_NOT_IMPLEMENTED;
*/
    }
}


int main(int argc, char** argv) {
    time_t start, end;
    // Se validan los parametros de entrada
    switch (ArgumentValidator_validate(argc, argv)) {
        case ERROR_CODE_NO_COMPRESS_INPUT_FILE:
            printf(ERROR_MSG_NO_COMPRESS_INPUT_FILE);
            break;
        case ERROR_CODE_NO_DECOMPRESS_INPUT_FILE:
            printf(ERROR_MSG_NO_DECOMPRESS_INPUT_FILE);
            break;
        case ERROR_CODE_INVALID_ARGUMENTS:
            printf(ERROR_MSG_INVALID_ARGUMENTS);
            break;
        case HELP_CODE:
            printf(HELP_MESSAGE);
            break;
        // Si pasa la validacion
        default:
            // Se comienza a contabilizar el tiempo
            time(&start);
            char* inputFileName = argv[2];
            size_t inputFileNameLength = strlen(inputFileName);
            char outputFileName[inputFileNameLength + strlen(FILE_OUTPUT_EXTENSION) + 1];
            int hasCompressArg = ArgumentValidator_hasCompressArg(argc, argv);
            // Si se quiere comprimir
            if (hasCompressArg) {
                // Construyo el nombre del archivo de salida
                strcpy(outputFileName, inputFileName);
                strcpy(outputFileName + inputFileNameLength, FILE_OUTPUT_EXTENSION);
            // Si se quiere descomprimir
            } else {
                inputFileNameLength -= strlen(FILE_OUTPUT_EXTENSION);
                strncpy(outputFileName, inputFileName, inputFileNameLength);
                outputFileName[inputFileNameLength] = '\0';
            }
            // Comprimo o descomprimo mostrando el mensaje de error si es necesario
//            switch (process("untitled.bmp", "untitled.bmp.XX", ALG_BSMTF_DHUFFMAN, 1, BUFFER_SIZE, WINDOW_SIZE)) {
            switch (process(inputFileName, outputFileName, ArgumentValidator_getCompressAlgoritm(argc, argv), hasCompressArg, BUFFER_SIZE, WINDOW_SIZE)) {
                case ERROR_CODE_INPUT_FILE_NOT_FOUND:
                    printf("%s %s\n", ERROR_MSG_INPUT_FILE_NOT_FOUND, inputFileName);
                    break;
                case ERROR_CODE_CANT_CREATE_OUTPUT_FILE:
                    printf("%s %s\n", ERROR_MSG_CANT_CREATE_OUTPUT_FILE, outputFileName);
                    break;
                case ERROR_CODE_NOT_FOUND:
                    printf(ERROR_MSG_CODE_NOT_FOUND);
                    break;
                case ERROR_CODE_DECOMPRESSOR_NOT_IMPLEMENTED:
                    printf(ERROR_MSG_DECOMPRESSOR_NOT_IMPLEMENTED);
                    break;
                case ERROR_CODE_OUT_OF_MEMORY:
                    printf(ERROR_MSG_OUT_OF_MEMORY);
                    break;
            }
            // Se imprime el tiempo que se tardo en comprimir
            time(&end);
            double diff = difftime(end, start);
            printf("\nTiempo: %dmin %dseg\n\n", (int)(diff/60), (int)((long)diff%60));
            break;
    }
    return 0;
}

