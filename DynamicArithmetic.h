/* 
 * File:   DynamicArithmetic.h
 * Author: Carlos Illobre
 *
 * Created on 2 de abril de 2011, 05:34
 */

#ifndef DYNAMICARITHMETIC_H
#define	DYNAMICARITHMETIC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "BufferedBitReader.h"
#include "BufferedWriter.h"

    #define CEILFLOORTYPE_MAX ULLONG_MAX
    typedef unsigned long long int CeilFloorType;
    typedef long double DivisionType;
//    #define CEILFLOORTYPE_MAX UCHAR_MAX
//    typedef unsigned char CeilFloorType;
//    typedef float DivisionType;

    //typedef unsigned int UnderflowType;

    typedef struct DynamicArithmetic {
        // Array con las frecuencias de todos los simbolos
        unsigned int* freq;
        // Cantidad de simbolos que se pueden comprimir
        unsigned int size;
        // Techo
        CeilFloorType ceil;
        // Piso
        CeilFloorType floor;
        // Cantidad de simbolos leidos hasta el momento
        unsigned int count;
        // Contador de underflow
        //UnderflowType underflow;
        // Numero leido hasta el momento para descomprimir
        CeilFloorType number;
        // flag, es true si se esta descomprimiendo y false en otro caso
        unsigned char isDecompress;
    } DynamicArithmetic;


    void DynamicArithmetic_create(DynamicArithmetic* this, unsigned int size);
    void DynamicArithmetic_destroy(DynamicArithmetic* this);

    /**
     * Escribe en el archivo el simbolo comprimido
     * @param this
     * @param symbol Simbolo que se quiere comprimir
     * @param writer Archivo donde se guardara symbol
     */
    void DynamicArithmetic_compress(DynamicArithmetic* this, unsigned int symbol, BufferedWriter* writer);

    /**
     * Escribe en el archivo el eof
     * @param this
     * @param writer
     */
    void DynamicArithmetic_compressEof(DynamicArithmetic* this, BufferedWriter* writer);

    /**
     *
     * @param this
     * @param reader
     * @return 1 si se pudo leer el caracter o 0 si no hay mas caracteres para leer
     */
    int DynamicArithmetic_decompress(DynamicArithmetic* this, unsigned int* symbol, BufferedBitReader* reader);

    /**
     * Incrementa en 1 la frecuencia de un simbolo
     * @param this
     * @param symbol
     */
    void DynamicArithmetic_incFreq(DynamicArithmetic* this, unsigned int symbol);


#ifdef	__cplusplus
}
#endif

#endif	/* DYNAMICARITHMETIC_H */

