/*
 * File:   Queue.h
 * Author: Carlos Illobre
 *
 * Created on 13 de marzo de 2011, 05:16
 */

#ifndef QUEUE_H
#define	QUEUE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "SimpleNode.h"


    typedef struct Queue {
        SimpleNode* head;
        SimpleNode* tail;
    } Queue;

    inline void Queue_create(Queue* this);
    void Queue_destroy(Queue* this);
    int Queue_push(Queue* this, void* data);
    void* Queue_pop(Queue* this);

    /**
     *
     * @param this
     * @return true si la cola esta vacia y false en otro caso
     */
    inline int Queue_isEmpty(const Queue* this);


#ifdef	__cplusplus
}
#endif

#endif	/* QUEUE_H */

