#ifndef __concurrent_queue_hpp__
#define __concurrent_qeueu_hpp__

#include "threadCommon.hpp"

namespace ConcurrentQueue {
typedef struct __node_t {
    int value;
    struct __node_t *next;
} node_t;

typedef struct __queue_t {
    node_t *head;
    node_t *tail;
    pthread_mutex_t headLock, tailLock;
} queue_t;

void init(queue_t *);
void enqueue(queue_t *, int);
int dequeue(queue_t *, int *);
int del(queue_t *);

} // namespace ConcurrentQueue

#endif