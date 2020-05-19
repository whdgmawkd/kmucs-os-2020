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
fstream file;

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

void approximateCounter(int threadCount, int factor) {
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
    double end = getSeconds();
    int ret = ConcurrentQueue::del(&queue);
    printf("ConcurrentQueue Result : %d, Time : %lf\n", ret, end - start);
    file << "ConcurrentQueue," << threadCount << "," << insertCount << "," << end - start << ",0" << endl;
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

void concurrentHashTable(int threadCount, int insertCount, bool sorted) {
    ConcurrentHashTable::hash_t hash;
    ConcurrentHashTable::init(&hash);
    printf("ConcurrentHashTable Test(%d:%d:%s)\n", threadCount, insertCount / threadCount * threadCount, sorted ? "Sorted" : "Front");
    double start = getSeconds();
    for (int i = 0; i < threadCount; i++) {
        pthread_create(threads + i, NULL, concurrentHashTableWorker, new pair<ConcurrentHashTable::hash_t *, pair<int, int>>(&hash, {i, insertCount / threadCount}));
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    double end = getSeconds();
    int ret = ConcurrentHashTable::del(&hash);
    printf("ConcurrentHashTable Result : %d, Time : %lf\n", ret, end - start);
    file << "ConcurrentHashTable," << threadCount << "," << insertCount << "," << end - start << ",0" << endl;
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

void lockCoupling(int threadCount, int insertCount, bool sorted) {
    LockCoupling::list_t l;
    LockCoupling::init(&l);
    printf("LockCoupling Test(%d:%d:%s)\n", threadCount, insertCount / threadCount * threadCount, sorted ? "Sorted" : "Front");
    double start = getSeconds();
    for (int i = 0; i < threadCount; i++) {
        pthread_create(threads + i, NULL, lockCouplingWorker, new pair<LockCoupling::list_t *, pair<int, int>>(&l, {i, insertCount / threadCount}));
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    double end = getSeconds();
    int ret = LockCoupling::del(&l);
    printf("LockCoupling Result : %d, Time : %lf\n", ret, end - start);
    file << "LockCoupling," << threadCount << "," << insertCount << "," << end - start << ",0" << endl;
}

int main(int argc, char *argv[]) {

    file.open("output.txt", ios::out | ios::trunc);
    if (!file.is_open()) {
        return -1;
    }

    file << "type,threadCount,insertCount,time,treshold" << endl;
    for (int loop = 0; loop < 4; loop++) {
        for (int i = 1; i <= NUMCPUS; i++)
            lockCounter(i);
        for (int i = 1; i <= NUMCPUS; i++)
            approximateCounter(i, 1024);
        for (int i = 1; i <= 30; i++)
            approximateCounter(NUMCPUS, 1 << i);
        for (int i = 1; i <= NUMCPUS; i++)
            concurrentQueue(NUMCPUS, 1000000 * i);
        for (int i = 1; i <= NUMCPUS; i++)
            concurrentHashTable(NUMCPUS, 1000000 * i, false);
        // for (int i = 1; i <= NUMCPUS; i++)
        //     concurrentHashTable(NUMCPUS, 10000 * i, true);
        for (int i = 1; i <= NUMCPUS; i++)
            lockCoupling(NUMCPUS, 1000000 * i, false);
        // for (int i = 1; i <= NUMCPUS; i++)
        //     lockCoupling(NUMCPUS, 10000 * i, true);
    }
    file.close();
    return 0;
}