#include <fstream>
#include <iostream>

#include "approximateCounter.hpp"
#include "concurrentHashTable.hpp"
#include "concurrentQueue.hpp"
#include "lockCounter.hpp"
#include "lockCoupling.hpp"
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

void lockCounter(int threadCount, fstream &file) {
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
    double end = getSeconds();
    printf("LockCounter Result : %d, Time : %lf\n", LockCounter::get(&counter), end - start);
    file << "LockCounter," << threadCount << "," << 1000000 * threadCount << "," << end - start << ",0" << endl;
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

void approximateCounter(int threadCount, int factor, fstream &file) {
    ApproximateCounter::counter_t counter;
    ApproximateCounter::init(&counter, factor);
    printf("ApproximateCounter Test(%d:%d)\n", threadCount, factor);
    double start = getSeconds();
    for (int i = 0; i < threadCount; i++) {
        pthread_create(threads + i, NULL, approximateCounterWorker, new pair<ApproximateCounter::counter_t *, int>(&counter, i));
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    double end = getSeconds();
    printf("ApproximateCounter Result : %d, Time : %lf\n", ApproximateCounter::total(&counter), end - start);
    file << "ApproximateCounter," << threadCount << "," << 1000000 * threadCount << "," << end - start << "," << factor << endl;
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

void concurrentQueue(int threadCount, int insertCount, fstream &file) {
    ConcurrentQueue::queue_t queue;
    ConcurrentQueue::init(&queue);
    printf("ConcurrentQueue Test(%d:%d)\n", threadCount, insertCount * threadCount);
    double start = getSeconds();
    for (int i = 0; i < threadCount; i++) {
        pthread_create(threads + i, NULL, concurrentQueueWorker, new pair<ConcurrentQueue::queue_t *, pair<int, int>>(&queue, {i, insertCount}));
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    double end = getSeconds();
    int ret = ConcurrentQueue::del(&queue);
    printf("ConcurrentQueue Result : %d, Time : %lf\n", ret, end - start);
    file << "ConcurrentQueue," << threadCount << "," << insertCount << "," << end - start << endl;
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

void concurrentHashTable(int threadCount, int insertCount, fstream &file) {
    ConcurrentHashTable::hash_t hash;
    ConcurrentHashTable::init(&hash);
    printf("ConcurrentHashTable Test(%d:%d)\n", threadCount, insertCount * threadCount);
    double start = getSeconds();
    for (int i = 0; i < threadCount; i++) {
        pthread_create(threads + i, NULL, concurrentHashTableWorker, new pair<ConcurrentHashTable::hash_t *, pair<int, int>>(&hash, {i, insertCount}));
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    double end = getSeconds();
    int ret = ConcurrentHashTable::del(&hash);
    printf("ConcurrentHashTable Result : %d, Time : %lf\n", ret, end - start);
    file << "ConcurrentHashTable," << threadCount << "," << insertCount << "," << end - start << endl;
}

void *lockCouplingWorker(void *args) {
    pair<LockCoupling::list_t *, pair<int, int>> *p = (pair<LockCoupling::list_t *, pair<int, int>> *)args;
    LockCoupling::list_t *l = p->first;
    int tid = p->second.first;
    int insertCount = p->second.second;
    int offset = insertCount * tid;
#ifdef DEBUG
    printf("Thread %d Started\n", tid);
#endif
    for (int i = 0; i < insertCount; i++) {
        LockCoupling::insert(l, offset + i);
    }
    return nullptr;
}

void lockCoupling(int threadCount, int insertCount, fstream &file) {
    LockCoupling::list_t l;
    LockCoupling::init(&l);
    printf("LockCoupling Test(%d:%d)\n", threadCount, insertCount * threadCount);
    double start = getSeconds();
    for (int i = 0; i < threadCount; i++) {
        pthread_create(threads + i, NULL, lockCouplingWorker, new pair<LockCoupling::list_t *, pair<int, int>>(&l, {i, insertCount}));
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    double end = getSeconds();
    int ret = LockCoupling::del(&l);
    printf("LockCoupling Result : %d, Time : %lf\n", ret, end - start);
    file << "LockCoupling," << threadCount << "," << insertCount << "," << end - start << endl;
}

void *concurrentLinkedWorker(void *args) {
    pair<ConcurrentLinked::list_t *, pair<int, int>> *p = (pair<ConcurrentLinked::list_t *, pair<int, int>> *)args;
    ConcurrentLinked::list_t *l = p->first;
    int tid = p->second.first;
    int insertCount = p->second.second;
    int offset = insertCount * tid;
#ifdef DEBUG
    printf("Thread %d Started\n", tid);
#endif
    for (int i = 0; i < insertCount; i++) {
        ConcurrentLinked::insert(l, offset + i);
    }
    return nullptr;
}

void concurrentLinked(int threadCount, int insertCount, fstream &file) {
    ConcurrentLinked::list_t l;
    ConcurrentLinked::init(&l);
    printf("ConcurreneLinked Test(%d:%d)\n", threadCount, insertCount * threadCount);
    double start = getSeconds();
    for (int i = 0; i < threadCount; i++) {
        pthread_create(threads + i, NULL, concurrentLinkedWorker, new pair<ConcurrentLinked::list_t *, pair<int, int>>(&l, {i, insertCount}));
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    double end = getSeconds();
    int ret = ConcurrentLinked::del(&l);
    printf("ConcurrentLinked Result %d, Time %lf\n", ret, end - start);
    file << "ConcurrentList," << threadCount << "," << insertCount << "," << end - start << endl;
}

int main(int argc, char *argv[]) {

    fstream figure295, figure296, figure2911;
    figure295.open("figure295.csv", ios::out | ios::trunc);
    figure296.open("figure296.csv", ios::out | ios::trunc);
    figure2911.open("figure2911.csv", ios::out | ios::trunc);

    figure295 << "type, threadCount, insertCount, time, threshold" << endl;
    figure296 << "type, threadCount, insertCount, time, threshold" << endl;
    figure2911 << "type, threadCount, insertCount, time" << endl;

    // // figure 29.5
    for (int loop = 0; loop < 5; loop++) {
        for (int i = 1; i <= NUMCPUS; i++) {
            lockCounter(i, figure295);
            approximateCounter(i, 1024, figure295);
        }
    }
    // figure 29.6
    for (int loop = 0; loop < 5; loop++) {
        for (int j = 0; j < 31; j++) {
            approximateCounter(48, 1 << j, figure296);
            approximateCounter(24, 1 << j, figure296);
            approximateCounter(12, 1 << j, figure296);
            approximateCounter(6, 1 << j, figure296);
        }
    }
    //figure 29.11
    for (int loop = 0; loop < 5; loop++) {
        for (int i = 1; i <= 20; i++) {
            // concurrent queue
            concurrentQueue(12, 10000 * i, figure2911);
            // lock coupling
            lockCoupling(12, 10000 * i, figure2911);
            // linked list
            concurrentLinked(12, 10000 * i, figure2911);
            // concurreht hash table
            concurrentHashTable(12, 10000 * i, figure2911);
        }
        for (int i = 1; i <= 20; i++) {
            // concurrent queue
            concurrentQueue(NUMCPUS, 10000 * i, figure2911);
            // lock coupling
            lockCoupling(NUMCPUS, 10000 * i, figure2911);
            // linked list
            concurrentLinked(NUMCPUS, 10000 * i, figure2911);
            // concurreht hash table
            concurrentHashTable(NUMCPUS, 10000 * i, figure2911);
        }
    }
    figure295.close();
    figure296.close();
    figure2911.close();
    return 0;
}