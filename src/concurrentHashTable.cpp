#include "concurrentHashTable.hpp"

namespace ConcurrentHashTable {

void init(hash_t *h) {
    for (int i = 0; i < BUCKETS; i++) {
        ConcurrentLinked::init(&h->lists[i]);
    }
}

int insert(hash_t *h, int key) {
    return ConcurrentLinked::insert(&h->lists[key % BUCKETS], key);
}

int lookup(hash_t *h, int key) {
    return ConcurrentLinked::lookup(&h->lists[key % BUCKETS], key);
}

int size(hash_t *h) {
    int count = 0;
    for (int i = 0; i < BUCKETS; i++) {
        count += ConcurrentLinked::size(&h->lists[i % BUCKETS]);
    }
    return count;
}

} // namespace ConcurrentHashTable