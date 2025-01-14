
/**
 * Implementation of thread pool.
 */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3
#define TRUE 1

typedef struct {
    void (*function)(void *p);
    void *data;
} task;

task worktodo[QUEUE_SIZE];
int queue_front = 0;
int queue_rear = 0;
int queue_len = 0;

pthread_t threads[NUMBER_OF_THREADS];
pthread_mutex_t queue_mutex;
sem_t task_sem; //semaforo das tarefas
int stop_pool = 0; //flag que encerra a pool

int enqueue(task t) {
    pthread_mutex_lock(&queue_mutex); //entra no mutex
    if (queue_len == QUEUE_SIZE) { //verifica a fila cheia
        pthread_mutex_unlock(&queue_mutex);
        return 1;
    }
    worktodo[queue_rear] = t; //coloca a task no fim da fila
    queue_rear = (queue_rear + 1) % QUEUE_SIZE;
    queue_len++;
    pthread_mutex_unlock(&queue_mutex); //sai do mutex
    sem_post(&task_sem); //incremento de semaforo
    return 0;
}

task dequeue() {
    pthread_mutex_lock(&queue_mutex); //entra no mutex
    task t = worktodo[queue_front]; //retira a task da frente da fila
    queue_front = (queue_front + 1) % QUEUE_SIZE;
    queue_len--;
    pthread_mutex_unlock(&queue_mutex); //sai do mutex
    return t; //retorna a task
}

void *worker(void *param) {
    while (TRUE) {
        sem_wait(&task_sem);

        if (stop_pool) { //para a pool
            break;
        }

        task t = dequeue();
        execute(t.function, t.data);
    }
    pthread_exit(0);
}

void execute(void (*somefunction)(void *p), void *p) {
    (*somefunction)(p);
}

int pool_submit(void (*somefunction)(void *p), void *p) {
    //criar uma nova task e adicionar na fila
    task new_task;
    new_task.function = somefunction;
    new_task.data = p;
    return enqueue(new_task);
}

void pool_init(void) {
    pthread_mutex_init(&queue_mutex, NULL);
    sem_init(&task_sem, 0, 0);
    stop_pool = 0;
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, NULL);
    }
}

void pool_shutdown(void) {
    pthread_mutex_lock(&queue_mutex);
    stop_pool = 1;
    pthread_mutex_unlock(&queue_mutex);

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        sem_post(&task_sem); //incrementar o semaforo para que as threads nao fiquem esperando
    }

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_join(threads[i], NULL); //aguardar as threads terminarem suas tarefas pra finalizar
    }

    pthread_mutex_destroy(&queue_mutex);
    sem_destroy(&task_sem);
}