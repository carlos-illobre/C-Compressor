/* 
 * File:   SimpleNode.h
 * Author: Carlos Illobre
 *
 * Created on 30 de marzo de 2011, 22:47
 */

#ifndef SIMPLENODE_H
#define	SIMPLENODE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdlib.h>


    typedef struct SimpleNode {
        void* data;
        struct SimpleNode* next;
    } SimpleNode;
    

    inline size_t SimpleNode_sizeOf();


#ifdef	__cplusplus
}
#endif

#endif	/* SIMPLENODE_H */

