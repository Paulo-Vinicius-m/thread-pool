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

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task worktodo [QUEUE_SIZE];
int tasks_on_queue = 0;

// the worker bee
pthread_t bees [NUMBER_OF_THREADS];

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    // execute the task
    execute(worktodo.function, worktodo.data);

    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{

    if (tasks_on_queue == QUEUE_SIZE){
        return 1;
    }

    

    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    for (int i=0; i<NUMBER_OF_THREADS; i++){
        pthread_create(&bees[i],NULL,worker,NULL);
    }
}

// shutdown the thread pool
void pool_shutdown(void)
{
    pthread_join(bee,NULL);
}
