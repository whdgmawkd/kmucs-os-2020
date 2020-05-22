#include "concurrentLinked.hpp"

namespace ConcurrentLinked {

void init(list_t *l) {
    l->head = nullptr;
    pthread_mutex_init(&l->lock, NULL);
}

int insert(list_t *l, int key) {
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    if (newNode == nullptr) {
        return -1;
    }
    newNode->key = key;
    pthread_mutex_lock(&l->lock);
    newNode->next = l->head;
    l->head = newNode;
    pthread_mutex_unlock(&l->lock);
    return 0;
}

int lookup(list_t *l, int key) {
    int rv = -1;
    pthread_mutex_lock(&l->lock);
    node_t *cur = l->head;
    while (cur) {
        if (cur->key == key) {
            rv = 0;
            break;
        }
        cur = cur->next;
    }
    pthread_mutex_unlock(&l->lock);
    return rv;
}

int del(list_t *l) {
    int cnt = 0;
    node_t *cur = l->head;
    while (cur) {
        node_t *delNode = cur;
        cur = cur->next;
        free(delNode);
        cnt++;
    }
    return cnt;
}

} // namespace ConcurrentLinked