#include <limits.h>
#include "DynamicArithmetic.h"


// Un 1 en el bit mas significativo y el resto con 0
static CeilFloorType ONE = 0;
// Un 0 en el bit mas significativo y el resto con 1
static CeilFloorType ZERO = 0;


void DynamicArithmetic_create(DynamicArithmetic* this, unsigned int size) {
    // Inicializo la constante ONE si no esta inicializada
    if (!ONE) {
        ONE = 1;
        unsigned int i;
        for (i=1; i<sizeof(CeilFloorType)*8; i++) ONE = ONE << 1;
    }
    if (!ZERO) ZERO = ONE-1;

    // Construyo el compresor
    this->freq = (unsigned int*)malloc(size * sizeof(unsigned int));
    if (this->freq) {
        this->size = size;
        unsigned int i;
        for (i=0; i<size; i++) {
            this->freq[i] = 1;
        }
        this->count = size;
        this->ceil = CEILFLOORTYPE_MAX;
        this->floor = 0;
        //this->underflow = 0;
        this->isDecompress = 0;
    } else {
        this->size = 0;
    }
}


void DynamicArithmetic_destroy(DynamicArithmetic* this) {
    free(this->freq);
}


/**
 * Escribe en el archivo los bits izquierdos que tienen en comun el techo y el piso
 * @param this
 * @param writer
 */
void DynamicArithmetic_write(DynamicArithmetic* this, BufferedWriter* writer) {
    // En la variable xor todos los bits izquierdos que coincidan entre piso y techo quedan en cero
    CeilFloorType xor;
    int bit;
    // Mientras el bit mas a la izquierda sea cero
    for (xor=this->ceil^this->floor; !(ONE & xor); xor=this->ceil^this->floor) {
        // Calculo el bit
        bit = (ONE & this->ceil) != 0;
        // Escribo en el archivo el bit mas significativo del techo
        BufferedWriter_writeBit(writer, bit);
        // Coloco un 0 a la derecha del piso
        this->floor = this->floor << 1;
        // Coloco un 1 a la derecha del techo
        this->ceil = this->ceil << 1;
        this->ceil++;
        // Si habia underflow escribo en el archivo tantos 0 como indique el underflow
/*
        while (this->underflow) {
            BufferedWriter_writeBit(writer, !bit);
            this->underflow--;
        }
*/
    }
}


/**
 * Lee del archivo los siguientes bits a procesar
 * @param this
 * @param writer
 */
int DynamicArithmetic_read(DynamicArithmetic* this, BufferedBitReader* reader) {
    int bit = 0;
    // Todos los bits izquierdos que coincidan entre piso y techo quedan en cero
    CeilFloorType xor;
    // Mientras el bit mas a la izquierda sea cero
    for (xor=this->ceil^this->floor; bit != -1 && !(ONE & xor); xor=this->ceil^this->floor) {
        bit=BufferedBitReader_readBit(reader);
        // Agrego un cero a la derecha
        this->number = this->number << 1;
        // Si el bit leido es un 1 transformo el cero de la derecha en un 1
        if (bit == 1) this->number++;
        // Coloco un 0 a la derecha del piso
        this->floor = this->floor << 1;
        // Coloco un 1 a la derecha del techo
        this->ceil = this->ceil << 1;
        this->ceil++;
    }
    return bit != -1;
}


/**
 * Calcula lso valores del techo y piso para un caracter dejando dichos valores
 * en ceil y floor respectivamente
 * @param this
 * @param symbol
 * @param ceil
 * @param floor
 */
void DynamicArithmetic_calculateCeilFloor(DynamicArithmetic* this, unsigned int symbol, CeilFloorType* ceil, CeilFloorType* floor) {
    // Constante que multiplica a la suma de las frecuencias
    DivisionType aux = ((DivisionType)(this->ceil-this->floor))/((DivisionType)(this->count));

    // Calculo la suma de las frecuencias de los simbolos anteriores
    unsigned int freqs = 0;
    unsigned int i;
    for (i=0; i<symbol; i++) freqs += this->freq[i];

    // Ceil(symbol) = Floor + (Ceil - Floor)*Sum(Prob(i)) con i=0 hasta i=symbol
    *ceil = this->floor + aux * (freqs + this->freq[symbol]);

    // Floor(symbol) = Floor + (Ceil - Floor)*Sum(Prob(i)) con i=0 hasta i=symbol-1
    *floor = this->floor + aux * freqs;
}


/* Hay underflow en el siguiente caso:
 * Ceil  = 10000000
 * Floor = 01111111
 * No se puede subdividir el intervalo debido a la falta de precision.
 *
 * Para evitar este caso hay que prevenirlo:
 * Ceil  = 10011010
 * Floor = 01101101  <- Segundo y tercer bit mas significativo en riesgo de underflow
 *
 * Cuento la cantidad de casos de underflow, en este caso son 2, por el segundo y tercer bit mas significativo
 * Underflow = 2
 *
 * Elimino los bits de riesgo de underflow, que son el segundo y el tercero
 * y relleno a la derecha con 1 para el techo y 0 para el piso
 * Ceil  = 11101011
 * Floor = 00110100
 * Underflow = 2
 *
 * Ahora se comprime, supongamos que queda lo siguiente:
 * Ceil  = 11100101
 * Floor = 11011010
 * Underflow = 2
 *
 * Emito el bit mas significativo que coincide, en este caso es 1, y relleno a
 * la derecha con 1 para el techo y 0 para el piso.
 * Ceil  = 11001011
 * Floor = 10110100
 * Underflow = 2
 * Emision = 1
 *
 * El bit mas significativo sigue coincidiendo, pero antes de emitir la segunda
 * coincidencia hay que emitir los undrflow. Como el bit emitido previamente fue
 * 1 hay que emitir tantos 0 como indique el underflow, si el bit emitido hubiese
 * sido 0 habria que emitir tantos 1 como indique el underflow
 * Ceil  = 11001011
 * Floor = 10110100
 * Underflow = 0
 * Emision = 100
 *
 * Ahora puedo emitir el resto de los bit mas significativos que coincidan
 * Ceil  = 10010111
 * Floor = 01101000
 * Underflow = 0
 * Emision = 1001
 */
/*
void DynamicArithmetic_renormalize(DynamicArithmetic* this) {
    // Elimino el primer bit del techo momentaneamente sabiendo que siempre sera 1, y relleno con un 1 a la izquierda
    this->ceil = this->ceil << 1;
    this->ceil++;
    // Elimino el primer bit del piso momentaneamente sabiendo que siempre sera 0, y relleno con un 0 a la izquierda
    this->floor = this->floor << 1;
    // Mientras el bit que quedo mas significativo en el techo sea 0 y
    // el bit que quedo mas significativo en el piso sea 1 hay riesgo de underflow
    while (!(ONE & this->ceil) && (ONE & this->floor)) {
        // incremento el contador de underflow
        this->underflow++;
        // Elimino el bit mas significativo del techo rellenando con un 1
        this->ceil = this->ceil << 1;
        this->ceil++;
        // Elimino el bit mas significativo del piso y relleno con un 0
        this->floor = this->floor << 1;
    }
    // Vuelvo a colocar en el techo el 1 quitado al principio en el bit mas significativo
    this->ceil = this->ceil >> 1;
    this->ceil = this->ceil | ONE;
    // Vuelvo a colocar en el piso el 0 quitado al principio en el bit mas significativo
    this->floor = this->floor >> 1;
    this->floor = this->floor & ZERO;
}
*/


void DynamicArithmetic_compress(DynamicArithmetic* this, unsigned int symbol, BufferedWriter* writer) {
    // Renormaliza calculando el underflow
    //DynamicArithmetic_renormalize(this);
    // Calculo los nuevos piso y techo
    DynamicArithmetic_calculateCeilFloor(this, symbol, &(this->ceil), &(this->floor));
    // Escribo en el archivo los bits mas significativos que tienen en comun el techo y el piso
    DynamicArithmetic_write(this, writer);
    // Incremento la frecuencia el simbolo
    DynamicArithmetic_incFreq(this, symbol);
}


void DynamicArithmetic_compressEof(DynamicArithmetic* this, BufferedWriter* writer) {
    // Escribo un 0 para que el numero sea menor que el techo
    BufferedWriter_writeBit(writer, 0);
    // Escribo tantos 1 como sean necesarios para que sea mayor que el piso
    // Mientras el bit mas a la izquierda del piso sea 1
    for (this->floor = this->floor << 1; ONE & this->floor; this->floor = this->floor << 1) {
        // Escribo en el archivo un 1
        BufferedWriter_writeBit(writer, 1);
    }
    BufferedWriter_writeBit(writer, 1);
}


int DynamicArithmetic_searchSymbol(DynamicArithmetic* this, unsigned int* symbol) {
    unsigned int low, high;
    CeilFloorType ceil, floor;
    low = 0;
    high = this->size-1;
    while (low <= high) {
        // Calculo techo y piso del symbolo
        *symbol = (low+high)/2;
        DynamicArithmetic_calculateCeilFloor(this, *symbol, &ceil, &floor);
        // Si el piso es mayor al numero el symbolo esta en la mitad inferior
        if (this->number < floor) {
            high = (*symbol)-1;
        // Si el techo es menor al numero entonces el simbolo esta en la mitad superior
        } else if (this->number >= ceil) {
            low = (*symbol)+1;
        // Si el numero es mayor o igual al piso y es menor que el techo
        } else {
            // Actualizo techo y piso con los techo y piso del simbolo encontrado
            this->ceil = ceil;
            this->floor = floor;
            return 1;
        }
    }
    // No se encontro el simbolo
    return 0;
}


int DynamicArithmetic_decompress(DynamicArithmetic* this, unsigned int* symbol, BufferedBitReader* reader) {
    int bit;
    unsigned int i;
    // Si previamente no se comenzo a descomprimir
    if (!this->isDecompress) {
        // Se comenzo a descomprimir
        this->isDecompress = 1;
        // Leo todos los bits del numero
        for (i=0, bit=BufferedBitReader_readBit(reader); i<sizeof(CeilFloorType)*8; i++, bit=BufferedBitReader_readBit(reader)) {
            // Agrego un cero a la derecha
            this->number = this->number << 1;
            // Si el bit leido es un 1 transformo el cero de la derecha en un 1
            if (bit == 1) this->number++;
        }
    }

    // Busco el simbolo correspondiente al numero actualizando techo y piso
    DynamicArithmetic_searchSymbol(this, symbol);

    // Leo los siguientes bits del archivo y los coloco en el numero
    int eof = DynamicArithmetic_read(this, reader);

/*
    // Renormalizo por si hay riesgo de underflow
    DynamicArithmetic_renormalize(this);
    // Quito el primer bit del numero almacenandolo
    bit = (this->number & ONE) ? 1 : 0;
    this->number = this->number << 1;
    // Mientras haya underflow elimino los bits que estaban segundos en el numero
    while (this->underflow) {
        if (BufferedReader_readBit(reader) == 1) this->number++;
        this->number = this->number << 1;
        this->underflow--;
    }
    // Vuelvo a colocar el bit quitado previamente
    this->number = bit ? (this->number >> 1) | ONE : (this->number >> 1) & ZERO;
*/

    // Incremento la frecuencia del symbolo encontrado
    DynamicArithmetic_incFreq(this, *symbol);

    return eof;
}


void DynamicArithmetic_incFreq(DynamicArithmetic* this, unsigned int symbol) {
    this->freq[symbol]++;
    this->count++;
    // Si se llego al limite de los caracteres que se pueden contar
    if (this->count == UINT_MAX) {
        // Busco la frecuencia minima
        unsigned int minFreq = UINT_MAX;
        unsigned int i;
        for (i=0; i<this->size; i++) {
            // Si incremento todas las frecuencias 1 para que la frecuencia
            // minima pueda dividir al resto de las frecuencias
            if (this->freq[i] == 1) {
                this->freq[i]++;
            }
            if (this->freq[i] < minFreq) {
                minFreq = this->freq[i];
            }
        }
        // Divido todas las frecuencias por la frecuencia minima para que se
        // mantenga la proporcion de frecuencias
        for (i=0; i<this->size; i++) {
            this->freq[i] /= minFreq;
        }
        // Divido la cantidad de caracteres leidos hasta el momento por la
        // frecuencia minima para que se mantengan las probabilidades
        this->count /= minFreq;
    }
}
