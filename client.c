/**
 * Example client program that uses thread pool.
 */
#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

struct data {
    int a;
    int b;
};

void add(void *param) {
    struct data *temp = (struct data *)param;
    printf("I add two values %d and %d, result = %d\n", temp->a, temp->b, temp->a + temp->b);
}

void mul(void *param) {
    struct data *temp = (struct data *)param;
    printf("I multiply two values %d and %d, result = %d\n", temp->a, temp->b, temp->a * temp->b);
}

void sub(void *param) {
    struct data *temp = (struct data *)param;
    printf("I subtract two values %d and %d, result = %d\n", temp->a, temp->b, temp->a - temp->b);
}

int main(void) {
    struct data work1 = {5, 10};
    struct data work2 = {20, 4};
    struct data work3 = {15, 7};
    struct data work4 = {7, 11};
    struct data work5 = {32, 6};
    struct data work6 = {4, 2};
    struct data work7 = {9, 19};
    struct data work8 = {3, 65};
    struct data work9 = {91, 13};

    // inicializar a pool de threads
    pool_init();

    // enviar as tarefas para a fila
    pool_submit(&add, &work1);
    pool_submit(&mul, &work2);
    pool_submit(&sub, &work3);
    pool_submit(&add, &work4);
    pool_submit(&mul, &work5);
    pool_submit(&sub, &work6);
    pool_submit(&add, &work7);
    pool_submit(&mul, &work8);
    pool_submit(&sub, &work9);

    // dormir por um tempo para permitir que as tarefas sejam executadas
    sleep(5);

    // desligar a pool de threads
    pool_shutdown();

    return 0;
}
