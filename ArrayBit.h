/* 
 * File:   ArrayBit.h
 * Author: Carlos Illobre
 *
 * Created on 22 de marzo de 2011, 23:12
 */

#ifndef ARRAYBIT_H
#define	ARRAYBIT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdlib.h>

    typedef struct ArrayBit {
        unsigned char* array;
        size_t length;
        size_t size;
    } ArrayBit;

    /**
     * Crea un arraybit listo para usar
     * @param this
     * @param size
     * @return
     */
    int ArrayBit_create(ArrayBit* this, size_t size);
    
    /**
     * Vacia el array sin destruirlo dejandolo listo para usar
     * @param this
     */
    inline void ArrayBit_init(ArrayBit* this);

    /**
     * Destruye el arraybit liberando los recursos utilizado
     * @param this
     */
    void ArrayBit_destroy(ArrayBit* this);

    /**
     *
     * @param this
     * @param other
     * @return true si ambos ArrayBit son iguales y false en otro caso
     */
    int ArrayBit_equals(const ArrayBit* this, const ArrayBit* other);

    /**
     * Agrega un bit al final del array
     * @param this
     * @param bit
     */
    int ArrayBit_addLast(ArrayBit* this, int bit);

    /**
     *
     * @param i
     * @return El valor del i-esimo bit o -1 en caso de que no exista un i-esimo bit
     */
    inline int ArrayBit_get(const ArrayBit* this, size_t i);

#ifdef	__cplusplus
}
#endif

#endif	/* ARRAYBIT_H */

