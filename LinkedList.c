#include <stdlib.h>
#include "LinkedList.h"


inline void LinkedList_create(LinkedList* this) {
    this->head = NULL;
    this->size = 0;
}


void LinkedList_destroy(LinkedList* this) {
    while(!LinkedList_isEmpty(this)) LinkedList_removeFirst(this);
    this->size = 0;
}


void* LinkedList_get(const LinkedList* this, size_t i) {
    int j;
    SimpleNode* cursor;
    for (j=0, cursor=this->head; cursor && j<i; j++, cursor=cursor->next);
    return cursor && i==j ? cursor->data : NULL;
}


inline void* LinkedList_getFirst(const LinkedList* this) {
    return this->head ? this->head->data : NULL;
}


int LinkedList_addFirst(LinkedList* this, void* data) {
    SimpleNode* node = (SimpleNode*) malloc(SimpleNode_sizeOf());
    if (node) {
        node->next = this->head;
        node->data = data;
        this->head = node;
        this->size++;
        return 1;
    }
    return 0;
}


int LinkedList_addSort(LinkedList* this, void* data, int (*compareTo)(const void*, const void*)) {
    // Si la lista esta vacia o data es menor que el primer elemento agrego el dato al principio
    if (LinkedList_isEmpty(this) || (*compareTo)(this->head->data, data) <= 0) return LinkedList_addFirst(this, data);
    // Busco el nodo que tenga como siguiente al dato mayor
    SimpleNode* cursor;
    for (cursor=this->head; cursor->next && (*compareTo)(cursor->next->data, data) >= 0; cursor=cursor->next);
    SimpleNode* n = (SimpleNode*) malloc(SimpleNode_sizeOf());
    if (n) {
        n->next = cursor->next;
        n->data = data;
        cursor->next = n;
        this->size++;
        return 1;
    }
    return 0;
}


void LinkedList_merge(LinkedList* this, LinkedList* other, int (*compareTo)(const void*, const void*)) {
    SimpleNode* thisNode = this->head;
    SimpleNode* otherNode = other->head;
    this->size += other->size;
    other->size = 0;

    // Si la otra lista esta vacia no hay nada para mergear
    if (!otherNode) return;

    // Si la lista esta vacia el merge es la otra lista
    if (!thisNode) {
        this->head = otherNode;
        other->head = NULL;
        return;
    }

    // Si ninguna de las dos listas estan vacias
    if ((*compareTo)(thisNode->data, otherNode->data) >= 0) {
        // Si el primer nodo de la lista es el menor se mergea sobre la lista original
        thisNode = thisNode->next;
    } else {
        // Si el primer nodo de la otra lista es menor se mergea sobre la otra lista
        this->head = otherNode;
        otherNode = otherNode->next;
    }

    SimpleNode* cursor = this->head;

    while (thisNode && otherNode) {
        if ((*compareTo)(thisNode->data, otherNode->data) >= 0) {
            cursor->next = thisNode;
            cursor = thisNode;
            thisNode = thisNode->next;
        } else {
            cursor->next = otherNode;
            cursor = otherNode;
            otherNode = otherNode->next;
        }
    }

    cursor->next = NULL;

    if (thisNode) {
        cursor->next = thisNode;
        cursor = thisNode;
        thisNode = thisNode->next;
    }

    if (otherNode) {
        cursor->next = otherNode;
        cursor = otherNode;
        otherNode = otherNode->next;
    }

    other->head = NULL;
}


void LinkedList_divide(LinkedList* this, LinkedList* other) {
    int i;
    SimpleNode* prev;
    for (other->head=this->head, i=0; i<this->size/2; i++, prev=other->head, other->head=other->head->next);
    other->size = this->size - this->size/2;
    this->size /= 2;
    prev->next = NULL;
}


void LinkedList_sort(LinkedList* this, int (*compareTo)(const void*, const void*)) {
    // Si la lista esta vacia o tiene un solo elemento ya esta ordenada
    if (!this->head || !this->head->next) return;
    // Divido la lista en dos mitades
    LinkedList other;
    LinkedList_divide(this, &other);
    // Ordeno una mitad
    LinkedList_sort(this, (*compareTo));
    // Ordeno la otra mitad
    LinkedList_sort(&other, (*compareTo));
    // se reconstruye la lista ordenada
    LinkedList_merge(this, &other, (*compareTo));
}


void* LinkedList_removeFirst(LinkedList* this) {
    if (!LinkedList_isEmpty(this)) {
        SimpleNode* n = this->head;
        this->head = n->next;
        void* data = n->data;
        free(n);
        this->size--;
        return data;
    }
    return NULL;
}


inline int LinkedList_isEmpty(const LinkedList* this) {
    return !this->head;
}


int LinkedList_moveToFront(LinkedList* this, void* data, int(*compare)(void*, void*)) {
    // Si el dato esta en el primer nodo retorno 0
    if (!(*compare)(data, this->head->data)) return 0;
    // Busco el nodo anterior al nodo que tiene el dato
    int pos = 1;
    SimpleNode* cursor;
    for (cursor=this->head; cursor->next && (*compare)(data, cursor->next->data); cursor=cursor->next, pos++);
    // Si no se encontro el dato retorno -1
    if (!cursor->next) return -1;
    // Coloco el nodo del dato al principio de la lista
    SimpleNode* aux = cursor->next;
    cursor->next = aux->next;
    aux->next = this->head;
    this->head = aux;
    return pos;
}