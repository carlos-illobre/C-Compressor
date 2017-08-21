/*
 * File:   ArgumentValidatorTest.c
 * Author: Carlos Illobre
 *
 * Created on 11/03/2011, 22:54:59
 */

#include <stdio.h>
#include <stdlib.h>
#include "ArgumentValidator.h"

#define NOMBRE_ARCHIVO_A_COMPRIMIR "pepe.txt"
#define NOMBRE_ARCHIVO_COMPRIMIDO NOMBRE_ARCHIVO_A_COMPRIMIR ".XX"

const char* ARGV_DOBLE_ARGUMENTO_VALIDO[] = {"ejecutable", ARG_COMPR, ARG_DESC, NOMBRE_ARCHIVO_A_COMPRIMIR};
const int ARGC_DOBLE_ARGUMENTO_VALIDO = 4;
const char* ARGV_COMPRIMIR_VALIDO[] = {"ejecutable", ARG_COMPR, NOMBRE_ARCHIVO_A_COMPRIMIR};
const int ARGC_COMPRIMIR_VALIDO = 3;
const char* ARGV_COMPRIMIR_SIN_ARCHIVO[] = {"ejecutable", ARG_COMPR};
const int ARGC_COMPRIMIR_SIN_ARCHIVO = 2;
const char* ARGV_DESCOMPRIMIR_VALIDO[] = {"ejecutable", ARG_DESC, NOMBRE_ARCHIVO_COMPRIMIDO};
const int ARGC_DESCOMPRIMIR_VALIDO = 3;
const char* ARGV_DESCOMPRIMIR_SIN_ARCHIVO[] = {"ejecutable", ARG_DESC};
const int ARGC_DESCOMPRIMIR_SIN_ARCHIVO = 2;
const char* ARGV_INVALIDO[] = {"ejecutable", "-d", NOMBRE_ARCHIVO_A_COMPRIMIR};
const int ARGC_INVALIDO = 3;
const char* ARGV_INVALIDO_SIN_ARCHIVO[] = {"ejecutable", "-d"};
const int ARGC_INVALIDO_SIN_ARCHIVO = 2;
const char* ARGV_SIN_ARGUMENTOS[] = {"ejecutable"};
const int ARGC_SIN_ARGUMENTOS = 1;

void testValidarComprimirSinArchivo() {
    int result = ArgumentValidator_validate(ARGC_COMPRIMIR_SIN_ARCHIVO, ARGV_COMPRIMIR_SIN_ARCHIVO);
    if (result !=  ERROR_CODE_NO_COMPRESS_INPUT_FILE) {
        printf("%%TEST_FAILED%% time=0 testname=testValidarComprimirSinArchivo (ArgumentValidatorTest) message=testValidarComprimirSinArchivo\n");
    }
}

void testValidarComprimir() {
    if (ArgumentValidator_validate(ARGC_COMPRIMIR_VALIDO, ARGV_COMPRIMIR_VALIDO) != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testValidarComprimir (ArgumentValidatorTest) message=testValidarComprimir\n");
    }
}

void testValidarDescomprimirSinArchivo() {
    int result = ArgumentValidator_validate(ARGC_DESCOMPRIMIR_SIN_ARCHIVO, ARGV_DESCOMPRIMIR_SIN_ARCHIVO);
    if (result != ERROR_CODE_NO_DECOMPRESS_INPUT_FILE) {
        printf("%%TEST_FAILED%% time=0 testname=testValidarDescomprimirSinArchivo (ArgumentValidatorTest) message=testValidarDescomprimirSinArchivo\n");
    }
}

void testValidarDobleArgumentoValido() {
    int result = ArgumentValidator_validate(ARGC_DOBLE_ARGUMENTO_VALIDO, ARGV_DOBLE_ARGUMENTO_VALIDO);
    if (result != ERROR_CODE_INVALID_ARGUMENTS) {
        printf("%%TEST_FAILED%% time=0 testname=testValidarDobleArgumentoValido (ArgumentValidatorTest) message=testValidarDobleArgumentoValido\n");
    }
}

void testValidarArgumentoInvalido() {
    int result = ArgumentValidator_validate(ARGC_INVALIDO, ARGV_INVALIDO);
    if (result !=  ERROR_CODE_INVALID_ARGUMENTS) {
        printf("%%TEST_FAILED%% time=0 testname=testValidarArgumentoInvalido (ArgumentValidatorTest) message=testValidarArgumentoInvalido\n");
    }
}

void testValidarDescomprimir() {
    if (ArgumentValidator_validate(ARGC_DESCOMPRIMIR_VALIDO, ARGV_DESCOMPRIMIR_VALIDO) != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testValidarDescomprimir (ArgumentValidatorTest) message=testValidarDescomprimir\n");
    }
}

void testValidarArgumentoInvalidoSinArchivo() {
    int result = ArgumentValidator_validate(ARGC_INVALIDO_SIN_ARCHIVO, ARGV_INVALIDO_SIN_ARCHIVO);
    if (result !=  ERROR_CODE_INVALID_ARGUMENTS) {
        printf("%%TEST_FAILED%% time=0 testname=testValidarArgumentoInvalidoSinArchivo (ArgumentValidatorTest) message=testValidarArgumentoInvalidoSinArchivo\n");
    }
}

void testValidarSinArgumentos() {
    int result = ArgumentValidator_validate(ARGC_SIN_ARGUMENTOS, ARGV_SIN_ARGUMENTOS);
    if (result !=  HELP_CODE) {
        printf("%%TEST_FAILED%% time=0 testname=testValidarSinArgumentos (ArgumentValidatorTest) message=testValidarSinArgumentos\n");
    }
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ArgumentValidatorTest\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testValidarComprimirSinArchivo (ArgumentValidatorTest)\n");
    testValidarComprimirSinArchivo();
    printf("%%TEST_FINISHED%% time=0 testValidarComprimirSinArchivo (ArgumentValidatorTest) \n");
    printf("%%TEST_STARTED%% testValidarComprimir (ArgumentValidatorTest)\n");
    testValidarComprimir();
    printf("%%TEST_FINISHED%% time=0 testValidarComprimir (ArgumentValidatorTest) \n");
    printf("%%TEST_STARTED%% testValidarDescomprimirSinArchivo (ArgumentValidatorTest)\n");
    testValidarDescomprimirSinArchivo();
    printf("%%TEST_FINISHED%% time=0 testValidarDescomprimirSinArchivo (ArgumentValidatorTest) \n");
    printf("%%TEST_STARTED%% testValidarDobleArgumentoValido (ArgumentValidatorTest)\n");
    testValidarDobleArgumentoValido();
    printf("%%TEST_FINISHED%% time=0 testValidarDobleArgumentoValido (ArgumentValidatorTest) \n");
    printf("%%TEST_STARTED%% testValidarArgumentoInvalido (ArgumentValidatorTest)\n");
    testValidarArgumentoInvalido();
    printf("%%TEST_FINISHED%% time=0 testValidarArgumentoInvalido (ArgumentValidatorTest) \n");
    printf("%%TEST_STARTED%% testValidarDescomprimir (ArgumentValidatorTest)\n");
    testValidarDescomprimir();
    printf("%%TEST_FINISHED%% time=0 testValidarDescomprimir (ArgumentValidatorTest) \n");
    printf("%%TEST_STARTED%% testValidarArgumentoInvalidoSinArchivo (ArgumentValidatorTest)\n");
    testValidarArgumentoInvalidoSinArchivo();
    printf("%%TEST_FINISHED%% time=0 testValidarArgumentoInvalidoSinArchivo (ArgumentValidatorTest) \n");
    printf("%%TEST_STARTED%% testValidarSinArgumentos (ArgumentValidatorTest)\n");
    testValidarSinArgumentos();
    printf("%%TEST_FINISHED%% time=0 testValidarSinArgumentos (ArgumentValidatorTest) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
