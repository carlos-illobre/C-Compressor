/*
 * File:   BufferedWriterTest.c
 * Author: Carlos Illobre
 *
 * Created on 12/03/2011, 19:55:00
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BufferedWriter.h"
#include "BufferedReader.h"
#include "BufferedBitReader.h"

void testBufferedWriterOpenAndClose() {
    const char* FILENAME = "pepe.txt";
    const size_t BUFFER_SIZE = 10;
    BufferedWriter* bufferedWriter = BufferedWriter_open(FILENAME, BUFFER_SIZE);
    // El buffer creado no debe ser NULL
    if (!bufferedWriter) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedWriterOpenAndClose (BufferedWriterTest) message=BufferedWriter == NULL\n");
        return;
    }
    // El archivo debe estar abierto
    else if (!bufferedWriter->file) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedWriterOpenAndClose (BufferedWriterTest) message=No se pudo abrir el archivo\n");
        return;
    }
    // El buffer no debe ser NULL
    else if (!bufferedWriter->buffer) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedWriterOpenAndClose (BufferedWriterTest) message=No se creo el buffer\n");
        return;
    }
    // El buffer debe tener el tamaño especificado
    else if (BUFFER_SIZE != bufferedWriter->bufferSize) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedWriterOpenAndClose (BufferedWriterTest) message=El tamaño del buffer no coincide\n");
        return;
    }
    // El cursor debe apuntar al primer elemento del buffer
    else if (bufferedWriter->cursor) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedWriterOpenAndClose (BufferedWriterTest) message=El cursor no apunta al primer elemento del buffer\n");
        return;
    }
    // Al cerrarse el bufferedReader debe haberse creado el archivo
    BufferedWriter_close(bufferedWriter);
    BufferedReader reader;
    BufferedReader_open(&reader, FILENAME, BUFFER_SIZE, 0);
    if (!reader.file) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedWriterOpenAndClose (BufferedWriterTest) message=No se creo el archivo\n");
        return;
    }
    BufferedReader_close(&reader);
    remove(FILENAME);
}

void testWriteImpar() {
    const char* FILENAME = "pepe.txt";
    char* FILECONTENT = "jerryrice";
    char* cursor;
    const size_t BUFFER_SIZE = (size_t)(strlen(FILECONTENT)/2);
    BufferedWriter* bufferedWriter = BufferedWriter_open(FILENAME, BUFFER_SIZE);
    if (!bufferedWriter) {
        printf("%%TEST_FAILED%% time=0 testname=testWriteImpar (BufferedWriterTest) message=No se pudo abrir el archivo\n");
        return;
    } else {
        for (cursor = FILECONTENT; *cursor; cursor++) {
            BufferedWriter_writeByte(bufferedWriter, *cursor);
        }
        BufferedWriter_close(bufferedWriter);
        // Comparo el contenido del archivo
        BufferedReader reader;
        BufferedReader_open(&reader, FILENAME, BUFFER_SIZE, 0);
        char c;
        for (cursor=FILECONTENT; BufferedReader_readNextByte(&reader, &c); cursor++) {
            if (c != *cursor) {
                printf("%%TEST_FAILED%% time=0 testname=testWriteImpar (BufferedWriterTest) message=El contenido del archivo no coincide\n");
                break;
            }
        }
        if (*cursor) {
            printf("%%TEST_FAILED%% time=0 testname=testWriteImpar (BufferedWriterTest) message=El archivo no se grabo en forma completa\n");
            return;
        }
        BufferedReader_close(&reader);
    }
    remove(FILENAME);
}

void testWritePar() {
    const char* FILENAME = "pepe.txt";
    char* FILECONTENT = "jerryricee";
    char* cursor;
    const size_t BUFFER_SIZE = (size_t)(strlen(FILECONTENT)/2);
    BufferedWriter* bufferedWriter = BufferedWriter_open(FILENAME, BUFFER_SIZE);
    if (!bufferedWriter) printf("%%TEST_FAILED%% time=0 testname=testWriteImpar (BufferedWriterTest) message=No se pudo abrir el archivo\n");
    else {
        for (cursor = FILECONTENT; *cursor; cursor++) {
            BufferedWriter_writeByte(bufferedWriter, *cursor);
        }
        BufferedWriter_close(bufferedWriter);
        // Comparo el contenido del archivo
        BufferedReader reader;
        BufferedReader_open(&reader, FILENAME, BUFFER_SIZE, 0);
        char c;
        for (cursor=FILECONTENT; BufferedReader_readNextByte(&reader, &c); cursor++) {
            if (c != *cursor) {
                printf("%%TEST_FAILED%% time=0 testname=testWriteImpar (BufferedWriterTest) message=El contenido del archivo no coincide\n");
                break;
            }
        }
        if (*cursor) printf("%%TEST_FAILED%% time=0 testname=testWriteImpar (BufferedWriterTest) message=El archivo no se grabo en forma completa\n");
        BufferedReader_close(&reader);
    }
    remove(FILENAME);
}


void testBufferedWriter_writeBit0() {
    const char* FILENAME = "pepe.txt";
    const size_t BUFFER_SIZE = 10;
    // Escribo un bit leo un bit cero
    BufferedWriter* writer = BufferedWriter_open(FILENAME, BUFFER_SIZE);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_closeWithEof(writer);
    BufferedBitReader reader;
    BufferedBitReader_open(&reader, FILENAME, BUFFER_SIZE);
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit0 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit0 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // Debe ser eof
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit0 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido eof\n");
            break;
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit0 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido eof\n");
            break;
    }
    BufferedBitReader_close(&reader);

    // Escribo un bit leo un bit cero
    writer = BufferedWriter_open(FILENAME, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_closeWithEof(writer);
    BufferedBitReader_open(&reader, FILENAME, 1);
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit0 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit0 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // Debe ser eof
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit0 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido eof\n");
            break;
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit0 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido eof\n");
            break;
    }
    BufferedBitReader_close(&reader);
    remove(FILENAME);
}


void testBufferedWriter_writeBit1() {
    const char* FILENAME = "pepe.txt";
    const size_t BUFFER_SIZE = 10;
    // Escribo un bit leo un bit uno
    BufferedWriter* writer = BufferedWriter_open(FILENAME, BUFFER_SIZE);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_closeWithEof(writer);
    BufferedBitReader reader;
    BufferedBitReader_open(&reader, FILENAME, BUFFER_SIZE);
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // Debe ser eof
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido eof\n");
            break;
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido eof\n");
            break;
    }
    BufferedBitReader_close(&reader);

    // Escribo un bit leo un bit uno
    writer = BufferedWriter_open(FILENAME, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_closeWithEof(writer);
    BufferedBitReader_open(&reader, FILENAME, 1);
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // Debe ser eof
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido eof\n");
            break;
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido eof\n");
            break;
    }
    BufferedBitReader_close(&reader);
    remove(FILENAME);
}


void testBufferedWriter_write3Bit() {
    const char* FILENAME = "pepe.txt";
    const size_t BUFFER_SIZE = 10;
    // Escribo un bit leo un bit uno
    BufferedWriter* writer = BufferedWriter_open(FILENAME, BUFFER_SIZE);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_closeWithEof(writer);
    BufferedBitReader reader;
    BufferedBitReader_open(&reader, FILENAME, BUFFER_SIZE);
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write3Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write3Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write3Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write3Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit0 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writeBit0 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    
    // Debe ser eof
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write3Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido eof\n");
            break;
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write3Bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido eof\n");
            break;
    }
    BufferedBitReader_close(&reader);
    remove(FILENAME);
}


void testBufferedWriter_write8Bit() {
    const char* FILENAME = "pepe.txt";
    const size_t BUFFER_SIZE = 10;
    // Escribo un bit leo un bit uno
    BufferedWriter* writer = BufferedWriter_open(FILENAME, BUFFER_SIZE);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_closeWithEof(writer);
    BufferedBitReader reader;

    BufferedBitReader_open(&reader, FILENAME, BUFFER_SIZE);
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }

    // Debe ser eof
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido eof\n");
            break;
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8Bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido eof\n");
            break;
    }
    BufferedBitReader_close(&reader);
    remove(FILENAME);
}


void testBufferedWriter_write8BitBufferSize1() {
    const char* FILENAME = "pepe.txt";
    const size_t BUFFER_SIZE = 1;
    // Escribo un bit leo un bit uno
    BufferedWriter* writer = BufferedWriter_open(FILENAME, BUFFER_SIZE);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_closeWithEof(writer);
    BufferedBitReader reader;

    BufferedBitReader_open(&reader, FILENAME, BUFFER_SIZE);
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }

    // Debe ser eof
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido eof\n");
            break;
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido eof\n");
            break;
    }
    BufferedBitReader_close(&reader);
    remove(FILENAME);
}


void testBufferedWriter_write9Bit() {
    const char* FILENAME = "pepe.txt";
    const size_t BUFFER_SIZE = 10;
    // Escribo un bit leo un bit uno
    BufferedWriter* writer = BufferedWriter_open(FILENAME, BUFFER_SIZE);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_closeWithEof(writer);
    BufferedBitReader reader;

    BufferedBitReader_open(&reader, FILENAME, BUFFER_SIZE);
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }

    // Debe ser eof
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido eof\n");
            break;
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9Bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido eof\n");
            break;
    }
    BufferedBitReader_close(&reader);
    remove(FILENAME);
}


void testBufferedWriter_write9BitBuffer1() {
    const char* FILENAME = "pepe.txt";
    const size_t BUFFER_SIZE = 1;
    // Escribo un bit leo un bit uno
    BufferedWriter* writer = BufferedWriter_open(FILENAME, BUFFER_SIZE);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_closeWithEof(writer);
    BufferedBitReader reader;

    BufferedBitReader_open(&reader, FILENAME, BUFFER_SIZE);
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }

    // Debe ser eof
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido eof\n");
            break;
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido eof\n");
            break;
    }
    BufferedBitReader_close(&reader);
    remove(FILENAME);
}


void testBufferedWriter_write24BitBuffer1() {
    const char* FILENAME = "pepe.txt";
    const size_t BUFFER_SIZE = 1;
    // Escribo un bit leo un bit uno
    BufferedWriter* writer = BufferedWriter_open(FILENAME, BUFFER_SIZE);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_closeWithEof(writer);
    BufferedBitReader reader;

    BufferedBitReader_open(&reader, FILENAME, BUFFER_SIZE);
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
        // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }

    // Debe ser eof
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido eof\n");
            break;
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido eof\n");
            break;
    }
    BufferedBitReader_close(&reader);
    remove(FILENAME);
}


void testBufferedWriter_write25BitBuffer1() {
    const char* FILENAME = "pepe.txt";
    const size_t BUFFER_SIZE = 1;
    // Escribo un bit leo un bit uno
    BufferedWriter* writer = BufferedWriter_open(FILENAME, BUFFER_SIZE);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 0);
    BufferedWriter_writeBit(writer, 1);
    BufferedWriter_closeWithEof(writer);
    BufferedBitReader reader;

    BufferedBitReader_open(&reader, FILENAME, BUFFER_SIZE);
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }
    // debe ser 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }

    // Debe ser eof
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido eof\n");
            break;
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido eof\n");
            break;
    }
    BufferedBitReader_close(&reader);
    remove(FILENAME);
}


void testBufferedWriter_writePrefix() {
    const char* FILENAME = "pepe.txt";
    const size_t BUFFER_SIZE = 1;
    size_t FILECONTENT[] = {6, 0, 1, 3, 4, 5, 2};
    int contentSize = 7;

    BufferedWriter* writer = BufferedWriter_open(FILENAME, BUFFER_SIZE);
    int i;
    for (i=0; i<contentSize; i++) {
        BufferedWriter_writePrefix(writer, FILECONTENT[i], 6);
    }
    BufferedWriter_closeWithEof(writer);

    BufferedBitReader reader;
    BufferedBitReader_open(&reader, FILENAME, BUFFER_SIZE);

    size_t n;
    for (i=0; i<contentSize; i++) {
        n = BufferedBitReader_readPrefix(&reader, 6);
        if (FILECONTENT[i] != n) {
            printf("%%TEST_FAILED%% time=0 testname=testBufferedWriter_writePrefix (BufferedWriterTest) message=%d) %d != %d\n", i, FILECONTENT[i], n);
        }
    }

    BufferedBitReader_close(&reader);
    remove(FILENAME);
}


void testBufferedBitReader_read8bit() {
    // Creo un archivo de prueba
    const char* FILENAME = "pepe.txt";
    unsigned char FILECONTENT[] = {0, 192, 64, 0, 80, 16, 8, 16, 20};
    int filecontentSize = 9;
    FILE* fp = fopen(FILENAME, "wb");
    fwrite(FILECONTENT, sizeof(unsigned char), filecontentSize, fp);
    fclose(fp);

    const size_t BUFFER_SIZE = 4;
    BufferedBitReader reader;
    BufferedBitReader_open(&reader, FILENAME, BUFFER_SIZE);
    unsigned char c;
    int i;

    // El primer bit es cero
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }

    // El proximo byte es 1
    i = BufferedBitReader_read8Bit(&reader, &c);
    if (i != 8) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyeron ocho bits, se leyeron %d\n", i);
    }
    if (c != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyo el byte 1, se leyo %d\n", c);
    }

    // El proximo bit es 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }

    // El proximo byte es 1
    i = BufferedBitReader_read8Bit(&reader, &c);
    if (i != 8) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyeron ocho bits, se leyeron %d\n", i);
    }
    if (c != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyo el byte 1, se leyo %d\n", c);
    }

    // El proximo byte es 0
    i = BufferedBitReader_read8Bit(&reader, &c);
    if (i != 8) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyeron ocho bits, se leyeron %d\n", i);
    }
    if (c != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyo el byte 0, se leyo %d\n", c);
    }

    // El proximo bit es 0
    switch (BufferedBitReader_readBit(&reader)) {
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido un cero\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un cero\n");
            break;
    }

    // El proximo byte es 2
    i = BufferedBitReader_read8Bit(&reader, &c);
    if (i != 8) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyeron ocho bits, se leyeron %d\n", i);
    }
    if (c != 2) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyo el byte 2, se leyo %d\n", c);
    }

    // El proximo bit es 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }

    // El proximo byte es 1
    i = BufferedBitReader_read8Bit(&reader, &c);
    if (i != 8) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyeron ocho bits, se leyeron %d\n", i);
    }
    if (c != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyo el byte 1, se leyo %d\n", c);
    }

    // El proximo byte es 0
    i = BufferedBitReader_read8Bit(&reader, &c);
    if (i != 8) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyeron ocho bits, se leyeron %d\n", i);
    }
    if (c != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyo el byte 0, se leyo %d\n", c);
    }

    // El proximo bit es 1
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido un uno\n");
            break;
        case -1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo eof cuando debria haberse leido un uno\n");
            break;
    }

    // El proximo byte es 2
    i = BufferedBitReader_read8Bit(&reader, &c);
    if (i != 8) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyeron ocho bits, se leyeron %d\n", i);
    }
    if (c != 2) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyo el byte 2, se leyo %d\n", c);
    }

    // El proximo byte es 2
    i = BufferedBitReader_read8Bit(&reader, &c);
    if (i != 8) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyeron ocho bits, se leyeron %d\n", i);
    }
    if (c != 2) {
        printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=No se leyo el byte 2, se leyo %d\n", c);
    }

    // Debe ser eof
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido eof\n");
            break;
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido eof\n");
            break;
    }
    // Debe ser eof
    switch (BufferedBitReader_readBit(&reader)) {
        case 0:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo un cero cuando debria haberse leido eof\n");
            break;
        case 1:
            printf("%%TEST_FAILED%% time=0 testname=testBufferedBitReader_read8bit (BufferedWriterTest) message=Se leyo un uno cuando debria haberse leido eof\n");
            break;
    }

    BufferedBitReader_close(&reader);
    remove(FILENAME);
}


void testWriteSize_t() {
    const char* FILENAME = "pepe.txt";
    size_t uno = 3;
    size_t dos = 256;
    size_t tres = 8000;
    size_t leido = 0;
    BufferedWriter* bufferedWriter = BufferedWriter_open(FILENAME, 6);
    if (!bufferedWriter) printf("%%TEST_FAILED%% time=0 testname=testWriteSize_t (BufferedWriterTest) message=No se pudo abrir el archivo\n");
    else {
        BufferedWriter_writeSize_t(bufferedWriter, uno);
        BufferedWriter_writeSize_t(bufferedWriter, dos);
        BufferedWriter_writeSize_t(bufferedWriter, tres);
        BufferedWriter_close(bufferedWriter);
        // Comparo el contenido del archivo
        BufferedReader reader;
        BufferedReader_open(&reader, FILENAME, 6, 0);
        if (!BufferedReader_readNextSize_t(&reader, &leido)) {
            printf("%%TEST_FAILED%% time=0 testname=testWriteSize_t (BufferedWriterTest) message=eof one\n");
        }
        if (leido != uno) printf("%%TEST_FAILED%% time=0 testname=testWriteSize_t (BufferedWriterTest) message=error en one\n");

        if (!BufferedReader_readNextSize_t(&reader, &leido)) {
            printf("%%TEST_FAILED%% time=0 testname=testWriteSize_t (BufferedWriterTest) message=eof dos\n");
        }
        if (leido != dos) printf("%%TEST_FAILED%% time=0 testname=testWriteSize_t (BufferedWriterTest) message=error en dos\n");

        if (!BufferedReader_readNextSize_t(&reader, &leido)) {
            printf("%%TEST_FAILED%% time=0 testname=testWriteSize_t (BufferedWriterTest) message=eof tres\n");
        }
        if (leido != tres) printf("%%TEST_FAILED%% time=0 testname=testWriteSize_t (BufferedWriterTest) message=error en tres\n");

        if (BufferedReader_readNextSize_t(&reader, &leido)) {
            printf("%%TEST_FAILED%% time=0 testname=testWriteSize_t (BufferedWriterTest) message=no es eof\n");
        }

        BufferedReader_close(&reader);
    }
    remove(FILENAME);
}


int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% BufferedWriterTest\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testBufferedWriterOpenAndClose (BufferedWriterTest)\n");
    testBufferedWriterOpenAndClose();
    printf("%%TEST_FINISHED%% time=0 testBufferedWriterOpenAndClose (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testWriteImpar (BufferedWriterTest)\n");
    testWriteImpar();
    printf("%%TEST_FINISHED%% time=0 testWriteImpar (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testWritePar (BufferedWriterTest)\n");
    testWritePar();
    printf("%%TEST_FINISHED%% time=0 testWritePar (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testBufferedWriter_writeBit0 (BufferedWriterTest)\n");
    testBufferedWriter_writeBit0();
    printf("%%TEST_FINISHED%% time=0 testBufferedWriter_writeBit0 (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testBufferedWriter_writeBit1 (BufferedWriterTest)\n");
    testBufferedWriter_writeBit1();
    printf("%%TEST_FINISHED%% time=0 testBufferedWriter_writeBit1 (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testBufferedWriter_write8Bit (BufferedWriterTest)\n");
    testBufferedWriter_write8Bit();
    printf("%%TEST_FINISHED%% time=0 testBufferedWriter_write8Bit (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testBufferedWriter_write8Bit (BufferedWriterTest)\n");
    testBufferedWriter_write8Bit();
    printf("%%TEST_FINISHED%% time=0 testBufferedWriter_write8Bit (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest)\n");
    testBufferedWriter_write8BitBufferSize1();
    printf("%%TEST_FINISHED%% time=0 testBufferedWriter_write8BitBufferSize1 (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testBufferedWriter_write9Bit (BufferedWriterTest)\n");
    testBufferedWriter_write9Bit();
    printf("%%TEST_FINISHED%% time=0 testBufferedWriter_write9Bit (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testBufferedWriter_write9BitBuffer1 (BufferedWriterTest)\n");
    testBufferedWriter_write9BitBuffer1();
    printf("%%TEST_FINISHED%% time=0 testBufferedWriter_write9BitBuffer1 (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testBufferedWriter_write24BitBuffer1 (BufferedWriterTest)\n");
    testBufferedWriter_write24BitBuffer1();
    printf("%%TEST_FINISHED%% time=0 testBufferedWriter_write24BitBuffer1 (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testBufferedWriter_write25BitBuffer1 (BufferedWriterTest)\n");
    testBufferedWriter_write25BitBuffer1();
    printf("%%TEST_FINISHED%% time=0 testBufferedWriter_write25BitBuffer1 (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testBufferedBitReader_read8bit (BufferedWriterTest)\n");
    testBufferedBitReader_read8bit();
    printf("%%TEST_FINISHED%% time=0 testBufferedBitReader_read8bit (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testBufferedWriter_writePrefix (BufferedWriterTest)\n");
    testBufferedWriter_writePrefix();
    printf("%%TEST_FINISHED%% time=0 testBufferedWriter_writePrefix (BufferedWriterTest) \n");

    printf("%%TEST_STARTED%% testWriteSize_t (BufferedWriterTest)\n");
    testWriteSize_t();
    printf("%%TEST_FINISHED%% time=0 testWriteSize_t (BufferedWriterTest) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
