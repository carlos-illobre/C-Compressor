/* 
 * File:   LinkedList.h
 * Author: Carlos Illobre
 *
 * Created on 23 de marzo de 2011, 20:16
 */

#ifndef LINKEDLIST_H
#define	LINKEDLIST_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "SimpleNode.h"
    

    typedef struct LinkedList {
        SimpleNode* head;
        size_t size;
    } LinkedList;

    /**
     * Inicializa la lista dejandola lista para usar
     * @param this
     */
    inline void LinkedList_create(LinkedList* this);

    /**
     * Destruye la lista liberando los recursos utilizados
     * @param this
     */
    void LinkedList_destroy(LinkedList* this);

    /**
     *
     * @param this
     * @param i
     * @return El i-esimo dato de la lista
     */
    void* LinkedList_get(const LinkedList* this, size_t i);

    /**
     *
     * @param this
     * @return El primer dato de la lista
     */
    inline void* LinkedList_getFirst(const LinkedList* this);

    /**
     * Agrega un dato al principio de la lista
     * @param this
     * @param data
     * @return true si pudo agregarse y false en otro caso
     */
    int LinkedList_addFirst(LinkedList* this, void* data);

    /**
     * Agrega un dato en la lista en una posicion tal que la lista quede ordenada
     * si es que previamente estaba ordenada
     * @param this
     * @param data
     * @param compareTo
     * @return true si pudo agregarse y false en otro caso
     */
    int LinkedList_addSort(LinkedList* this, void* data, int (*compareTo)(const void*, const void*));

    /**
     * En this que da una lista con los elementos de ambas listas. Si las listas
     * estaban ordenadas entonces this queda tambien ordenada
     * @param this
     * @param other
     * @param compareTo
     */
    void LinkedList_merge(LinkedList* this, LinkedList* other, int (*compareTo)(const void*, const void*));
    
    /**
     * Divide una lista en dos listas por la mitad
     * @param this
     * @param other
     */
    void LinkedList_divide(LinkedList* this, LinkedList* other);

    /**
     * Ordena una lista
     * @param this
     * @param compareTo
     */
    void LinkedList_sort(LinkedList* this, int (*compareTo)(const void*, const void*));

    /**
     * Elimina el primer elemento de la lista
     * @param this
     * @return
     */
    void* LinkedList_removeFirst(LinkedList* this);

    /**
     *
     * @param this
     * @return true si la lista esta vacia y false en otro caso
     */
    inline int LinkedList_isEmpty(const LinkedList* this);

    /**
     * Busca el elemento data en la lista y mueve su nodo al principio de la lista
     * @param this
     * @param data
     * @param compare Funcion que compara dos datos. Retorna cero si los datos
     * son iguales y un numeor distinto de cero si son distintos.
     * @return La posicion de la lista en donde estaba el dato o -1 si no se encontro
     * el dato.
     */
    int LinkedList_moveToFront(LinkedList* this, void* data, int(*compare)(void*, void*));


#ifdef	__cplusplus
}
#endif

#endif	/* LINKEDLIST_H */

