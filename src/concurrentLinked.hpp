#ifndef __concurrent_linked_hpp__
#define __concurrent_linked_hpp__

#include "threadCommon.hpp"

namespace ConcurrentLinked {

typedef struct __node_t {
    int key;
    struct __node_t *next;
} node_t;

typedef struct __list_t {
    node_t *head;
    pthread_mutex_t lock;
} list_t;

void init(list_t *);
int insert(list_t *, int);
int lookup(list_t *, int);
int del(list_t *);

} // namespace ConcurrentLinked

#endif