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

int del(hash_t *h){
    int cnt = 0;
    for(int i=0;i<BUCKETS;i++){
        cnt += ConcurrentLinked::del(&h->lists[i]);
    }
    return cnt;
}

} // namespace ConcurrentHashTable