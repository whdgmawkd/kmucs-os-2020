#include "lockCoupling.hpp"

namespace LockCoupling {

void init(list_t *l) {
    // first is dummy node
    l->head = (node_t *)malloc(sizeof(node_t));
    pthread_mutex_init(&l->head->lock, NULL);
    pthread_mutex_init(&l->lock, NULL);
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

int insertSorted(list_t *l, int key) {
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    if (newNode == nullptr) {
        return -1;
    }
    pthread_mutex_init(&newNode->lock, NULL);
    newNode->key = key;
    pthread_mutex_lock(&l->lock);
    node_t *prev = l->head;
    node_t *cur = l->head->next;
    pthread_mutex_lock(&prev->lock);
    pthread_mutex_unlock(&l->lock);
    if (cur) {
        pthread_mutex_lock(&cur->lock);
    }
    while (cur) {
        if (cur->key > key) {
            break;
        }
        node_t *temp = prev;
        prev = cur;
        cur = cur->next;
        pthread_mutex_unlock(&temp->lock);
        if (cur) {
            pthread_mutex_lock(&cur->lock);
        }
    }
    newNode->next = cur;
    prev->next = newNode;
    pthread_mutex_unlock(&prev->lock);
    if (cur) {
        pthread_mutex_unlock(&cur->lock);
    }
    return 0;
}

int lookup(list_t *l, int key) {
    int rv = -1;
    // prev is dummy node
    node_t *cur = l->head->next;
    node_t *prev = l->head;
    while (true) {
        // lock current
        // check key
        // lock next
        // move next (move current as prev)
        // unlock prev
    }
    while (cur) {
        pthread_mutex_lock(&cur->lock);
        if (cur->key == key) {
            pthread_mutex_unlock(&cur->lock);
            return 0;
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

int del(list_t *l) {
    node_t *cur = l->head;
    // dummy node
    int cnt = -1;
    while (cur) {
        node_t *delNode = cur;
        cur = cur->next;
        free(delNode);
        cnt++;
    }
    return cnt;
}

} // namespace LockCoupling