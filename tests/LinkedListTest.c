/*
 * File:   LinkedListTest.c
 * Author: Carlos Illobre
 *
 * Created on 23/03/2011, 21:02:04
 */

#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"


int testCompareTo(const void* this, const void* other) {
    return *((int*)other) - *((int*)this);
}


void testLinkedList_addFirst() {
    LinkedList l;
    LinkedList_create(&l);
    int data1 = 23;
    int data2 = 45;
    int data3 = 78;
    if (!LinkedList_addFirst(&l, &data1)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addFirst (LinkedListTest) message=No se agrego data1\n");
    if (l.size != 1) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addFirst (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 1);
        return;
    }
    if (!LinkedList_addFirst(&l, &data2)) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addFirst (LinkedListTest) message=No se agrego data1\n");
        return;
    }
    if (l.size != 2) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addFirst (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 2);
        return;
    }
    if (!LinkedList_addFirst(&l, &data3)) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addFirst (LinkedListTest) message=No se agrego data1\n");
        return;
    }
    if (l.size != 3) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addFirst (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 3);
        return;
    }
    int n = *((int*)LinkedList_get(&l, 0));
    if (n != data3) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addFirst (LinkedListTest) message=No agrego data3 al principio\n");
        return;
    }
    n = *((int*)LinkedList_get(&l, 1));
    if (n != data2) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addFirst (LinkedListTest) message=No agrego data2 al principio\n");
        return;
    }
    n = *((int*)LinkedList_get(&l, 2));
    if (n != data1) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addFirst (LinkedListTest) message=No agrego data1 al principio\n");
        return;
    }
    if (LinkedList_get(&l, 3)) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addFirst (LinkedListTest) message=Fallo el get con NULL\n");
        return;
    }
    LinkedList_destroy(&l);
    if (!LinkedList_isEmpty(&l)) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addFirst (LinkedListTest) message=No destruyo\n");
        return;
    }
    if (LinkedList_get(&l, 0)) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addFirst (LinkedListTest) message=Fallo el get con NULL\n");
        return;
    }
    if (l.size != 0) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addFirst (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 0);
        return;
    }
}


void testLinkedList_removeFirst() {
    LinkedList l;
    LinkedList_create(&l);
    int data1 = 23;
    int data2 = 45;
    int data3 = 78;
    if (!LinkedList_addFirst(&l, &data1)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_removeFirst (LinkedListTest) message=No se agrego data1\n");
    if (!LinkedList_addFirst(&l, &data2)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_removeFirst (LinkedListTest) message=No se agrego data1\n");
    if (!LinkedList_addFirst(&l, &data3)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_removeFirst (LinkedListTest) message=No se agrego data1\n");
    if (*(int*)LinkedList_removeFirst(&l) != data3) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_removeFirst (LinkedListTest) message=No agrego data3 al principio\n");
    if (l.size != 2) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_removeFirst (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 2);
    if (*(int*)LinkedList_removeFirst(&l) != data2) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_removeFirst (LinkedListTest) message=No agrego data2 al principio\n");
    if (l.size != 1) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_removeFirst (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 1);
    if (*(int*)LinkedList_removeFirst(&l) != data1) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_removeFirst (LinkedListTest) message=No agrego data1 al principio\n");
    if (l.size != 0) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_removeFirst (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 0);
    if (!LinkedList_isEmpty(&l)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_removeFirst (LinkedListTest) message=No destruyo\n");
}


void testLinkedList_addSort() {
    LinkedList l;
    LinkedList_create(&l);
    int data1 = 23;
    int data2 = 45;
    int data3 = 78;
    int data4 = 1;
    int data5 = 24;
    int data6 = 25;
    if (!LinkedList_addSort(&l, &data1, testCompareTo)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=No se agrego data1\n");
    if (l.size != 1) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 1);
    if (!LinkedList_addSort(&l, &data2, testCompareTo)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=No se agrego data2\n");
    if (l.size != 2) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 2);
    if (!LinkedList_addSort(&l, &data3, testCompareTo)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=No se agrego data3\n");
    if (l.size != 3) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 3);
    if (!LinkedList_addSort(&l, &data4, testCompareTo)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=No se agrego data4\n");
    if (l.size != 4) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 4);
    if (!LinkedList_addSort(&l, &data5, testCompareTo)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=No se agrego data5\n");
    if (l.size != 5) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 5);
    if (!LinkedList_addSort(&l, &data6, testCompareTo)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=No se agrego data6\n");
    if (l.size != 6) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 6);
    int prev, result;
    int size = 4;
    for (prev=*((int*)LinkedList_removeFirst(&l)), result=*((int*)LinkedList_removeFirst(&l)); !LinkedList_isEmpty(&l); prev=result, result=*((int*)LinkedList_removeFirst(&l)), size--) {
        if (prev > result) {
            printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=No estan ordenados: %d %d\n", prev, result);
            return;
        }
        if (l.size != size) {
            printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, size);
            return;
        }
    }
    LinkedList_destroy(&l);
    if (!LinkedList_isEmpty(&l)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSort (LinkedListTest) message=No destruyo\n");
}


void testLinkedList_addSortEqual() {
    LinkedList this;
    LinkedList_create(&this);
    int data1 = 23;
    int data2 = 23;
    int data3 = 25;
    if (!LinkedList_addSort(&this, &data1, testCompareTo)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSortEqual (LinkedListTest) message=No se agrego data1\n");
    if (this.size != 1) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSortEqual (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", this.size, 1);
    if (!LinkedList_addSort(&this, &data2, testCompareTo)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSortEqual (LinkedListTest) message=No se agrego data2\n");
    if (this.size != 2) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSortEqual (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", this.size, 2);
    if (!LinkedList_addSort(&this, &data3, testCompareTo)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSortEqual (LinkedListTest) message=No se agrego data3\n");
    if (this.size != 3) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSortEqual (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", this.size, 3);
    int prev, result;
    int size = 3;
    for (prev=*((int*)LinkedList_removeFirst(&this)), result=*((int*)LinkedList_removeFirst(&this)), size-=2; !LinkedList_isEmpty(&this); prev=result, result=*((int*)LinkedList_removeFirst(&this)), size--) {
        if (prev > result) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSortEqual (LinkedListTest) message=No estan ordenados: %d %d\n", prev, result);
        if (this.size != size) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSortEqual (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", this.size, size);
    }
    LinkedList_destroy(&this);
    if (!LinkedList_isEmpty(&this)) printf("%%TEST_FAILED%% time=0 testname=testLinkedList_addSortEqual (LinkedListTest) message=No destruyo\n");
}


void testLinkedList_sort() {
    LinkedList l;
    LinkedList_create(&l);
    int data1 = 23;
    int data2 = 45;
    int data3 = 78;
    int data4 = 1;
    int data5 = 24;
    int data6 = 25;
    int data7 = 2;
    LinkedList_addFirst(&l, &data1);
    LinkedList_addFirst(&l, &data2);
    LinkedList_addFirst(&l, &data3);
    LinkedList_addFirst(&l, &data4);
    LinkedList_addFirst(&l, &data5);
    LinkedList_addFirst(&l, &data6);
    LinkedList_addFirst(&l, &data7);
    LinkedList_sort(&l, testCompareTo);

    if (l.size != 7) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_sort (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 7);
        return;
    }

    int prev, result, size;
    for (size=2, prev=*((int*)LinkedList_removeFirst(&l)), result=*((int*)LinkedList_removeFirst(&l)); !LinkedList_isEmpty(&l); prev=result, result=*((int*)LinkedList_removeFirst(&l)), size++) {
        if (prev > result) {
            printf("%%TEST_FAILED%% time=0 testname=testLinkedList_sort (LinkedListTest) message=No estan ordenados: %d %d\n", prev, result);
            return;
        }
    }
    if (prev > result) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_sort (LinkedListTest) message=No estan ordenados: %d %d\n", prev, result);
        return;
    }

    if (size != 7) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_sort (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", size, 7);
        return;
    }

    LinkedList_destroy(&l);
}


void testLinkedList_isEmpty() {
    LinkedList this;
    LinkedList_create(&this);
    if (!LinkedList_isEmpty(&this)) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_isEmpty (LinkedListTest) message=No esta vacia al inicio\n");
        return;
    }
    int i = 1;
    LinkedList_addFirst(&this, &i);
    if (LinkedList_isEmpty(&this)) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_isEmpty (LinkedListTest) message=Se agrego un elemento y esta vacia\n");
        return;
    }
    LinkedList_addFirst(&this, &i);
    LinkedList_removeFirst(&this);
    if (LinkedList_isEmpty(&this)) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_isEmpty (LinkedListTest) message=Se agregaron dos elementos y se removio uno y esta vacia\n");
        return;
    }
    LinkedList_removeFirst(&this);
    if (!LinkedList_isEmpty(&this)) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_isEmpty (LinkedListTest) message=Se removieron los elementos y no esta vacia\n");
        return;
    }
    LinkedList_addFirst(&this, &i);
    if (LinkedList_isEmpty(&this)) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_isEmpty (LinkedListTest) message=Se agrego un elemento luego de vaciarla y sigue vacia\n");
        return;
    }
    LinkedList_destroy(&this);
    if (!LinkedList_isEmpty(&this)) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_isEmpty (LinkedListTest) message=Se destruyo y no esta vacia\n");
        return;
    }
}


void testLinkedList_merge() {
    LinkedList l, ll, m, mm, s, ss;
    LinkedList_create(&l);
    LinkedList_create(&ll);
    LinkedList_create(&m);
    LinkedList_create(&mm);
    LinkedList_create(&s);
    LinkedList_create(&ss);
    int data1 = 23;
    int data2 = 45;
    int data3 = 78;
    int data4 = 1;
    int data5 = 24;
    int data6 = 25;
    LinkedList_addSort(&l, &data1, testCompareTo);
    LinkedList_addSort(&l, &data2, testCompareTo);
    LinkedList_addSort(&l, &data3, testCompareTo);
    LinkedList_addSort(&m, &data1, testCompareTo);
    LinkedList_addSort(&m, &data2, testCompareTo);
    LinkedList_addSort(&m, &data3, testCompareTo);
    LinkedList_addSort(&ll, &data4, testCompareTo);
    LinkedList_addSort(&ll, &data5, testCompareTo);
    LinkedList_addSort(&ll, &data6, testCompareTo);
    LinkedList_addSort(&mm, &data4, testCompareTo);
    LinkedList_addSort(&s, &data4, testCompareTo);
    LinkedList_addSort(&ss, &data4, testCompareTo);
    LinkedList_addSort(&mm, &data5, testCompareTo);
    LinkedList_addSort(&mm, &data6, testCompareTo);
    LinkedList_merge(&l, &ll, testCompareTo);
    LinkedList_merge(&mm, &m, testCompareTo);

    if (l.size != 6) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_merge (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", l.size, 6);
        return;
    }

    if (l.size != mm.size) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_merge (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", mm.size, l.size);
        return;
    }

    size_t size;
    int prev, result;
    for (size=2, prev=*((int*)LinkedList_removeFirst(&l)), result=*((int*)LinkedList_removeFirst(&l)); !LinkedList_isEmpty(&l); prev=result, result=*((int*)LinkedList_removeFirst(&l)), size++) {
        if (prev > result) {
            printf("%%TEST_FAILED%% time=0 testname=testLinkedList_merge (LinkedListTest) message=No estan ordenados: %d %d\n", prev, result);
            return;
        }
    }
    if (prev > result) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_merge (LinkedListTest) message=No estan ordenados: %d %d\n", prev, result);
        return;
    }
    if (size != 6) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_merge (LinkedListTest) message=El size vale %d cuando deberia valer %d\n", size, 6);
        return;
    }

    for (; !LinkedList_isEmpty(&l); prev=*((int*)LinkedList_removeFirst(&l)), result=*((int*)LinkedList_removeFirst(&m))) {
        if (prev != result) {
            printf("%%TEST_FAILED%% time=0 testname=testLinkedList_merge (LinkedListTest) message=No son iguales: %d %d\n", prev, result);
            return;
        }
    }

    LinkedList_destroy(&l);
    LinkedList_destroy(&ll);
    LinkedList_destroy(&m);
    LinkedList_destroy(&mm);
    LinkedList_destroy(&s);
    LinkedList_destroy(&ss);
}


void testLinkedList_moveToFront() {
    LinkedList l;
    LinkedList_create(&l);
    int data1 = 23;
    int data2 = 45;
    int data3 = 78;
    int data4 = 1;
    int data5 = 24;
    int data6 = 25;
    int data7 = 2;
    LinkedList_addFirst(&l, &data1);
    LinkedList_addFirst(&l, &data2);
    LinkedList_addFirst(&l, &data3);
    LinkedList_addFirst(&l, &data4);
    LinkedList_addFirst(&l, &data5);
    LinkedList_addFirst(&l, &data6);
    LinkedList_addFirst(&l, &data7);

    int pos = LinkedList_moveToFront(&l, &data2, testCompareTo);
    if (pos != 5) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_moveToFront (LinkedListTest) message=La posicion retornada es %d pero deberia valer %d\n", pos, 5);
        return;
    }
    if (data2 != *((int*)LinkedList_getFirst(&l))) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_moveToFront (LinkedListTest) message=La posicion retornada es %d pero deberia valer %d\n", *((int*)LinkedList_getFirst(&l)), data2);
        return;
    }
    LinkedList_removeFirst(&l);
    if (data7 != *((int*)LinkedList_getFirst(&l))) {
        printf("%%TEST_FAILED%% time=0 testname=testLinkedList_moveToFront (LinkedListTest) message=La posicion retornada es %d pero deberia valer %d\n", *((int*)LinkedList_getFirst(&l)), data7);
        return;
    }

    LinkedList_destroy(&l);
}


int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% LinkedListTest\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%%  testLinkedList_addFirst (LinkedListTest)\n");
    testLinkedList_addFirst();
    printf("%%TEST_FINISHED%% time=0 testLinkedList_addFirst (LinkedListTest)\n");

    printf("%%TEST_STARTED%%  testLinkedList_removeFirst (LinkedListTest)\n");
    testLinkedList_removeFirst();
    printf("%%TEST_FINISHED%% time=0 testLinkedList_removeFirst (LinkedListTest)\n");

    printf("%%TEST_STARTED%%  testLinkedList_addSort (LinkedListTest)\n");
    testLinkedList_addSort();
    printf("%%TEST_FINISHED%% time=0 testLinkedList_addSort (LinkedListTest)\n");

    printf("%%TEST_STARTED%%  testLinkedList_addSortEqual (LinkedListTest)\n");
    testLinkedList_addSortEqual();
    printf("%%TEST_FINISHED%% time=0 testLinkedList_addSortEqual (LinkedListTest)\n");

    printf("%%TEST_STARTED%%  testLinkedList_isEmpty (LinkedListTest)\n");
    testLinkedList_isEmpty();
    printf("%%TEST_FINISHED%% time=0 testLinkedList_isEmpty (LinkedListTest)\n");

    printf("%%TEST_STARTED%%  testLinkedList_merge (LinkedListTest)\n");
    testLinkedList_merge();
    printf("%%TEST_FINISHED%% time=0 testLinkedList_merge (LinkedListTest)\n");

    printf("%%TEST_STARTED%%  testLinkedList_moveToFront (LinkedListTest)\n");
    testLinkedList_moveToFront();
    printf("%%TEST_FINISHED%% time=0 testLinkedList_moveToFront (LinkedListTest)\n");
    
    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
