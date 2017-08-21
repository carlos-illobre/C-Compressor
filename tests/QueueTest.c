/*
 * File:   QueueTest.c
 * Author: Carlos Illobre
 *
 * Created on 13/03/2011, 05:20:59
 */

#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

void testQueueCreate() {
    Queue q;
    Queue_create(&q);
    // La cola y la cabeza deben ser null
    if (q.head != NULL || q.tail != NULL) printf("%%TEST_FAILED%% time=0 testname=testQueueCreate (QueueTest) message=Mal inicializada\n");
    Queue_destroy(&q);
}

void testQueuePushPop() {
    int dato = 23;
    Queue q;
    Queue_create(&q);
    if (!Queue_push(&q, &dato)) printf("%%TEST_FAILED%% time=0 testname=testQueuePushPop (QueueTest) message=No se inserto el valor\n");
    int* resultado = (int*)Queue_pop(&q);
    if (dato != *resultado) printf("%%TEST_FAILED%% time=0 testname=testQueuePushPop (QueueTest) message=Valores diferentes\n");
    if (q.head || q.tail) printf("%%TEST_FAILED%% time=0 testname=testQueuePushPop (QueueTest) message=No se vacio\n");
    int otroDato = 45;
    int otroDatoMas = 89;
    Queue_push(&q, &dato);
    Queue_push(&q, &otroDato);
    Queue_push(&q, &otroDatoMas);
    resultado = (int*)Queue_pop(&q);
    int* otroResultado = (int*)Queue_pop(&q);
    int* otroResultadoMas = (int*)Queue_pop(&q);
    if (dato != *resultado) printf("%%TEST_FAILED%% time=0 testname=testQueuePushPop (QueueTest) message=Valores diferentes\n");
    if (otroDato != *otroResultado) printf("%%TEST_FAILED%% time=0 testname=testQueuePushPop (QueueTest) message=Mas valores diferentes\n");
    if (otroDatoMas != *otroResultadoMas) printf("%%TEST_FAILED%% time=0 testname=testQueuePushPop (QueueTest) message=Otro valor diferente mas\n");
    if (q.head || q.tail) printf("%%TEST_FAILED%% time=0 testname=testQueuePushPop (QueueTest) message=No se vacio\n");
    Queue_destroy(&q);
}

void testQueueEmptyPop() {
    Queue q;
    Queue_create(&q);
    if (Queue_pop(&q)) printf("%%TEST_FAILED%% time=0 testname=testQueuePushPop (QueueTest) message=Retorno algo\n");
    Queue_destroy(&q);
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% QueueTest\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testQueueCreate (QueueTest)\n");
    testQueueCreate();
    printf("%%TEST_FINISHED%% time=0 testQueueCreate (QueueTest) \n");

    printf("%%TEST_STARTED%% testQueuePushPop (QueueTest)\n");
    testQueuePushPop();
    printf("%%TEST_FINISHED%% time=0 testQueuePushPop (QueueTest) \n");

    printf("%%TEST_STARTED%% testQueueEmptyPop (QueueTest)\n");
    testQueueEmptyPop();
    printf("%%TEST_FINISHED%% time=0 testQueueEmptyPop (QueueTest) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
