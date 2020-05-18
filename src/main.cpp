#include <cstdio>
#include <iostream>

#include "approximateCounter.hpp"
#include "concurrentHashTable.hpp"
#include "concurrentQueue.hpp"
#include "lockCounter.hpp"
#include "threadCommon.hpp"

using namespace std;

pthread_t threads[NUMCPUS];

void *lockCounterWorker(void *args) {
    pair<LockCounter::counter_t *, int> *p = (pair<LockCounter::counter_t *, int> *)args;
    LockCounter::counter_t *counter = p->first;
    int tid = p->second;
#ifdef DEBUG
    printf("Thread %d Started\n", tid);
#endif
    for (int i = 0; i < 1000000; i++) {
        LockCounter::increment(counter);
    }
    return nullptr;
}

void lockCounter(int threadCount) {
    LockCounter::counter_t counter;
    LockCounter::init(&counter);
    printf("LockCounter Test(%d)\n", threadCount);
    double start = getSeconds();
    for (int i = 0; i < threadCount; i++) {
        pthread_create(threads + i, NULL, lockCounterWorker, new pair<LockCounter::counter_t *, int>(&counter, i));
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    printf("LockCounter Result : %d, Time : %lf\n", LockCounter::get(&counter), getSeconds() - start);
}

void *approximateCounterWorker(void *args) {
    pair<ApproximateCounter::counter_t *, int> *p = (pair<ApproximateCounter::counter_t *, int> *)args;
    ApproximateCounter::counter_t *counter = p->first;
    int tid = p->second;
#ifdef DEBUG
    printf("Thread %d Started\n", tid);
#endif
    for (int i = 0; i < 1000000; i++) {
        ApproximateCounter::update(counter, tid, 1);
    }
    return nullptr;
}

void approximateCounter(int threadCount) {
    ApproximateCounter::counter_t counter;
    ApproximateCounter::init(&counter, 1024);
    printf("ApproximateCounter Test(%d)\n", threadCount);
    double start = getSeconds();
    for (int i = 0; i < threadCount; i++) {
        pthread_create(threads + i, NULL, approximateCounterWorker, new pair<ApproximateCounter::counter_t *, int>(&counter, i));
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    printf("ApproximateCounter Result : %d, Time : %lf\n", ApproximateCounter::total(&counter), getSeconds() - start);
}

void *concurrentQueueWorker(void *args) {
    pair<ConcurrentQueue::queue_t *, pair<int, int>> *p = (pair<ConcurrentQueue::queue_t *, pair<int, int>> *)args;
    ConcurrentQueue::queue_t *q = p->first;
    int tid = p->second.first;
    int insertCount = p->second.second;
    int offset = tid * insertCount;
#ifdef DEBUG
    printf("Thread %d Started\n", tid);
#endif
    for (int i = 0; i < insertCount; i++) {
        ConcurrentQueue::enqueue(q, offset + i);
    }
    return nullptr;
}

void concurrentQueue(int threadCount, int insertCount) {
    ConcurrentQueue::queue_t queue;
    ConcurrentQueue::init(&queue);
    printf("ConcurrentQueue Test(%d:%d)\n", threadCount, insertCount / threadCount * threadCount);
    double start = getSeconds();
    for (int i = 0; i < threadCount; i++) {
        pthread_create(threads + i, NULL, concurrentQueueWorker, new pair<ConcurrentQueue::queue_t *, pair<int, int>>(&queue, {i, insertCount / threadCount}));
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    printf("ConcurrentQueue Result : %d, Time %lf\n", ConcurrentQueue::size(&queue), getSeconds() - start);
}

void *concurrentHashTableWorker(void *args) {
    pair<ConcurrentHashTable::hash_t *, pair<int, int>> *p = (pair<ConcurrentHashTable::hash_t *, pair<int, int>> *)args;
    ConcurrentHashTable::hash_t *h = p->first;
    int tid = p->second.first;
    int insertCount = p->second.second;
    int offset = insertCount * tid;
#ifdef DEBUG
    printf("Thread %d Started\n", tid);
#endif
    for (int i = 0; i < insertCount; i++) {
        ConcurrentHashTable::insert(h, offset + i);
    }
    return nullptr;
}

void concurrentHashTable(int threadCount, int insertCount) {
    ConcurrentHashTable::hash_t hash;
    ConcurrentHashTable::init(&hash);
    printf("ConcurrentHashTable Test(%d:%d)\n", threadCount, insertCount / threadCount * threadCount);
    double start = getSeconds();
    for (int i = 0; i < threadCount; i++) {
        pthread_create(threads + i, NULL, concurrentHashTableWorker, new pair<ConcurrentHashTable::hash_t *, pair<int, int>>(&hash, {i, insertCount / threadCount}));
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    printf("ConcurrentHashTable Result : %d, Time %lf\n", ConcurrentHashTable::size(&hash), getSeconds() - start);
}

int main(int argc, char *argv[]) {
    for (int i = 1; i <= NUMCPUS; i++)
        lockCounter(i);
    for (int i = 1; i <= NUMCPUS; i++)
        approximateCounter(i);
    for (int i = 1; i <= NUMCPUS; i++)
        concurrentQueue(NUMCPUS, 1000000 * i);
    for (int i = 1; i <= NUMCPUS; i++) {
        concurrentHashTable(NUMCPUS, 1000000 * i);
    }
    return 0;
}