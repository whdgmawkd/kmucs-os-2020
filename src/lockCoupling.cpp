#include "lockCoupling.hpp"

namespace LockCoupling {

void init(list_t *l) {
    l->head = nullptr;
}

int insert(list_t *l, int key) {
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    if (newNode == nullptr) {
        return -1;
    }
    newNode->key = key;
    pthread_mutex_lock(&l->head->lock);
    pthread_mutex_lock(&newNode->lock);
    newNode->next = l->head;
    l->head = newNode;
    pthread_mutex_unlock(&newNode->lock);
    pthread_mutex_unlock(&l->head->next->lock);
    return 0;
}

int lookup(list_t *l, int key) {
    int rv = -1;
    node_t *cur = l->head;
    while (cur) {
        pthread_mutex_lock(&cur->lock);
        if (cur->key == key) {
            rv = 0;
            break;
        }
        pthread_mutex_unlock(&cur->lock);
        cur = cur->next;
    }
    return rv;
}

} // namespace LockCoupling