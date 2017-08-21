#include <stdlib.h>
#include "Queue.h"


inline void Queue_create(Queue* this) {
    this->head = NULL;
    this->tail = NULL;
}


void Queue_destroy(Queue* this) {
    while (this->head) Queue_pop(this);
}


int Queue_push(Queue* this, void* data) {
    SimpleNode* n = (SimpleNode*)malloc(SimpleNode_sizeOf());
    if (n) {
        n->data = data;
        n->next = NULL;
        if (this->tail) {
            this->tail->next = n;
            this->tail = n;
        } else {
            this->tail = n;
            this->head = n;
        }
        return 1;
    }
    return 0;
}


void* Queue_pop(Queue* this) {
    void* data = NULL;
    if (this->head) {
        SimpleNode* n = this->head;
        this->head = n->next;
        if (!this->head) this->tail = NULL;
        data = n->data;
        free(n);
    }
    return data;
}


inline int Queue_isEmpty(const Queue* this) {
    return !this->head;
}
