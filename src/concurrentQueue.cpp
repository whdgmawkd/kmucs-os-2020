#include "concurrentQueue.hpp"

namespace ConcurrentQueue {

void init(queue_t *q) {
    node_t *tmp = (node_t *)malloc(sizeof(node_t));
    tmp->next = nullptr;
    q->head = q->tail = tmp;
    pthread_mutex_init(&q->headLock, NULL);
    pthread_mutex_init(&q->tailLock, NULL);
}

void enqueue(queue_t *q, int value) {
    node_t *tmp = (node_t *)malloc(sizeof(node_t));
    tmp->value = value;
    tmp->next = nullptr;

    pthread_mutex_lock(&q->tailLock);
    q->tail->next = tmp;
    q->tail = tmp;
    pthread_mutex_unlock(&q->tailLock);
}

int dequeue(queue_t *q, int *value) {
    node_t *tmp = q->head;
    node_t *newHead = tmp->next;
    if (newHead == nullptr) {

        return -1;
    }
    *value = newHead->value;
    pthread_mutex_unlock(&q->headLock);
    q->head = newHead;
    pthread_mutex_unlock(&q->headLock);
    free(tmp);
    return 0;
}

int size(queue_t *q) {
    int count = -1;
    node_t *cur = q->head;
    while (cur) {
        count++;
        cur = cur->next;
    }
    return count;
}

} // namespace ConcurrentQueue