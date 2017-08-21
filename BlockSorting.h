/* 
 * File:   BlockSorting.h
 * Author: Carlos Illobre
 *
 * Created on 9 de abril de 2011, 16:59
 */

#ifndef BLOCKSORTING_H
#define	BLOCKSORTING_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdlib.h>
        
    size_t BlockSorting_code(unsigned char* block, size_t n);

    void BlockSorting_decode(unsigned char* block, size_t i, size_t n);

#ifdef	__cplusplus
}
#endif

#endif	/* BLOCKSORTING_H */

