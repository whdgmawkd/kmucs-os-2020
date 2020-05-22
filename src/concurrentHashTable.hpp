#ifndef __concurrent_hash_table_hpp__
#define __concurrent_hash_table_hpp__

#include "concurrentLinked.hpp"
#include "threadCommon.hpp"

namespace ConcurrentHashTable {

const int BUCKETS = 101;

typedef struct __hash_t {
    ConcurrentLinked::list_t lists[BUCKETS];
} hash_t;

void init(hash_t *);
int insert(hash_t *, int);
int lookup(hash_t *, int);
int del(hash_t *);

} // namespace ConcurrentHashTable

#endif