#include "lockCoupling.hpp"

namespace LockCoupling {

void init(list_t *l) {
    // first is dummy node
    l->head = (node_t *)malloc(sizeof(node_t));
    pthread_mutex_init(&l->head->lock, NULL);
    l->head->key = -1;
    l->head->next = nullptr;
}

int insert(list_t *l, int key) {
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    if (newNode == nullptr) {
        return -1;
    }
    // init node lock
    pthread_mutex_init(&newNode->lock, NULL);
    newNode->key = key;
    pthread_mutex_lock(&l->head->lock);
    newNode->next = l->head->next;
    l->head->next = newNode;
    pthread_mutex_unlock(&l->head->lock);
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

int size(list_t *l) {
    int cnt = -1;
    node_t *cur = l->head;
    while (cur) {
        cnt++;
        cur = cur->next;
    }
    return cnt;
}

} // namespace LockCoupling