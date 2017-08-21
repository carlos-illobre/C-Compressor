/*
 * File:   ArrayBitTest.c
 * Author: Carlos Illobre
 *
 * Created on 23/03/2011, 03:00:02
 */

#include <stdio.h>
#include <stdlib.h>
#include "ArrayBit.h"


void testArrayBit_equals() {
    ArrayBit this, equal, distinct;
    ArrayBit_create(&this, 32);
    ArrayBit_create(&equal, 12);
    ArrayBit_create(&distinct, 32);
    if (!ArrayBit_equals(&this, &equal)) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_equals (ArrayBitTest) message=No son inicialmente iguales\n");
        return;
    }

    ArrayBit_addLast(&this, 1);
    ArrayBit_addLast(&this, 0);
    ArrayBit_addLast(&this, 1);
    ArrayBit_addLast(&this, 1);
    ArrayBit_addLast(&this, 1);
    ArrayBit_addLast(&this, 0);
    ArrayBit_addLast(&this, 0);
    ArrayBit_addLast(&this, 0);
    ArrayBit_addLast(&this, 1);

    ArrayBit_addLast(&equal, 1);
    ArrayBit_addLast(&equal, 0);
    ArrayBit_addLast(&equal, 1);
    ArrayBit_addLast(&equal, 1);
    ArrayBit_addLast(&equal, 1);
    ArrayBit_addLast(&equal, 0);
    ArrayBit_addLast(&equal, 0);
    ArrayBit_addLast(&equal, 0);
    ArrayBit_addLast(&equal, 1);

    ArrayBit_addLast(&distinct, 1);
    ArrayBit_addLast(&distinct, 0);
    ArrayBit_addLast(&distinct, 1);
    ArrayBit_addLast(&distinct, 1);
    ArrayBit_addLast(&distinct, 1);
    ArrayBit_addLast(&distinct, 0);
    ArrayBit_addLast(&distinct, 0);
    ArrayBit_addLast(&distinct, 0);
    ArrayBit_addLast(&distinct, 0);

    if (!ArrayBit_equals(&this, &equal)) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_equals (ArrayBitTest) message=No son iguales\n");
        return;
    }

    if (ArrayBit_equals(&this, &distinct)) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_equals (ArrayBitTest) message=No son distintos\n");
        return;
    }

    ArrayBit_destroy(&this);
    ArrayBit_destroy(&equal);
    ArrayBit_destroy(&distinct);
}


void testArrayBit_addFirst() {
    // Se crea un ArrayBit
    ArrayBit this;
    ArrayBit_create(&this, 32);
    if (!this.array || this.length || this.size != 32) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=No se creo\n");
        return;
    }

    // Agrego 011001011
    ArrayBit_addLast(&this, 1);
    if (this.length != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=Al agregarse no se incremento length: %d\n", this.length);
        return;
    }
    ArrayBit_addLast(&this, 1);
    if (this.length != 2) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=Al agregarse no se incremento length: %d\n", this.length);
        return;
    }
    ArrayBit_addLast(&this, 0);
    if (this.length != 3) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=Al agregarse no se incremento length: %d\n", this.length);
        return;
    }
    ArrayBit_addLast(&this, 1);
    if (this.length != 4) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=Al agregarse no se incremento length: %d\n", this.length);
        return;
    }
    ArrayBit_addLast(&this, 0);
    if (this.length != 5) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=Al agregarse no se incremento length: %d\n", this.length);
        return;
    }
    ArrayBit_addLast(&this, 0);
    if (this.length != 6) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=Al agregarse no se incremento length: %d\n", this.length);
        return;
    }
    ArrayBit_addLast(&this, 1);
    if (this.length != 7) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=Al agregarse no se incremento length: %d\n", this.length);
        return;
    }
    ArrayBit_addLast(&this, 1);
    if (this.length != 8) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=Al agregarse no se incremento length: %d\n", this.length);
        return;
    }
    ArrayBit_addLast(&this, 0);
    if (this.length != 9) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=Al agregarse no se incremento length: %d\n", this.length);
        return;
    }

    // Debe quedar 011001011
    if (ArrayBit_get(&this, 0) != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=La primera posicion no tiene un uno\n");
        return;
    }
    if (ArrayBit_get(&this, 1) != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=La segunda posicion no tiene un uno\n");
        return;
    }
    if (ArrayBit_get(&this, 2) != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=La tercera posicion no tiene un cero\n");
        return;
    }
    if (ArrayBit_get(&this, 3) != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=La cuarta posicion no tiene un uno\n");
        return;
    }
    if (ArrayBit_get(&this, 4) != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=La quinta posicion no tiene un cero\n");
        return;
    }
    if (ArrayBit_get(&this, 5) != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=La sexta posicion no tiene un cero\n");
        return;
    }
    if (ArrayBit_get(&this, 6) != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=La septima posicion no tiene un uno\n");
        return;
    }
    if (ArrayBit_get(&this, 7) != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=La octava posicion no tiene un uno\n");
        return;
    }
    if (ArrayBit_get(&this, 8) != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=La novena posicion no tiene un cero\n");
        return;
    }
    if (ArrayBit_get(&this, 9) != -1) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=Hay datos en la decima posicion\n");
        return;
    }

    if (!this.array || !this.length || this.size != 32) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=No se inserto nada\n");
        return;
    }

    // Se destruye el arraybit
    ArrayBit_destroy(&this);
    if (this.array || this.length || this.size) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_addFirst (ArrayBitTest) message=No se destruyo\n");
        return;
    }
}


void testArrayBit_init() {
    ArrayBit this;
    ArrayBit_create(&this, 32);

    ArrayBit_init(&this);
    if (!this.array || this.length || this.size != 32) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_init (ArrayBitTest) message=Fallo al inicializarse un array recien creado\n");
        return;
    }

    ArrayBit_addLast(&this, 1);
    ArrayBit_init(&this);
    if (!this.array || this.length || this.size != 32) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_init (ArrayBitTest) message=Fallo al inicializarse un array con un elemento\n");
        return;
    }

    if (ArrayBit_get(&this, 0) != -1) {
        printf("%%TEST_FAILED%% time=0 testname=testArrayBit_init (ArrayBitTest) message=El array contiene elementos luego de la inicializacion\n");
        return;
    }

    ArrayBit_destroy(&this);
}


int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ArrayBitTest\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testArrayBit_equals (ArrayBitTest)\n");
    testArrayBit_equals();
    printf("%%TEST_FINISHED%% time=0 testArrayBit_equals (ArrayBitTest) \n");

    printf("%%TEST_STARTED%% testArrayBit_addFirst (ArrayBitTest)\n");
    testArrayBit_addFirst();
    printf("%%TEST_FINISHED%% time=0 testArrayBit_addFirst (ArrayBitTest) \n");

    printf("%%TEST_STARTED%% testArrayBit_init (ArrayBitTest)\n");
    testArrayBit_init();
    printf("%%TEST_FINISHED%% time=0 testArrayBit_init (ArrayBitTest) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
