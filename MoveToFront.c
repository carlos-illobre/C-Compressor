#include "MoveToFront.h"
#include "LinkedList.h"


static int compare(void* this, void* other) {
    return *((unsigned char*)other) - *((unsigned char*)this);
}


int MoveToFront_create(MoveToFront* this) {
    LinkedList_create(&(this->l));
    int c;
    for (c=255; c>=0; c--) {
        this->characters[c] = c;
        // Si la lista no pudo crearse
        if (!LinkedList_addFirst(&(this->l), this->characters+c)) {
            // Destruyo lo que se creo de la lista
            LinkedList_destroy(&(this->l));
            // retorno 0
            return 0;
        }
    }
    return 1;
}


void MoveToFront_destroy(MoveToFront* this) {
    LinkedList_destroy(&(this->l));
}


void MoveToFront_code(MoveToFront* this, unsigned char* buffer, size_t n) {
    // Para cada caracter del buffer
    unsigned char pos;
    size_t i;
    for (i=0; i<n; i++) {
        // Busco la posicion del caracter del buffer en la lista y reemplazo
        // el caracter del buffer por la posicion del caracter en la lista
        pos = LinkedList_moveToFront(&(this->l), buffer+i, compare);
        buffer[i] = pos;
    }
}


void MoveToFront_decode(MoveToFront* this, unsigned char* buffer, size_t n) {
    // Para cada caracter del buffer
    size_t i;
    for (i=0; i<n; i++) {
        buffer[i] = *((unsigned char*)LinkedList_get(&(this->l), buffer[i]));
        LinkedList_moveToFront(&(this->l), buffer+i, compare);
    }
}
