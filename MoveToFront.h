/* 
 * File:   MoveToFront.h
 * Author: Carlos Illobre
 *
 * Created on 9 de abril de 2011, 02:57
 */

#ifndef MOVETOFRONT_H
#define	MOVETOFRONT_H

#include <stdlib.h>
#include "LinkedList.h"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct MoveToFront {
        unsigned char characters[256];
        LinkedList l;
    } MoveToFront;

    int MoveToFront_create(MoveToFront* this);
    void MoveToFront_destroy(MoveToFront* this);

    /**
     * Ejecuta el algoritmo MTF
     * @param buffer Caracteres a los que se les quiere aplicar el algoritmo
     * @param n Cantidad de caracters del buffer
     */
    void MoveToFront_code(MoveToFront* this, unsigned char* buffer, size_t n);

    void MoveToFront_decode(MoveToFront* this, unsigned char* buffer, size_t n);


#ifdef	__cplusplus
}
#endif

#endif	/* MOVETOFRONT_H */

