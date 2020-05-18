#ifndef __approximate_counter_hpp__
#define __approximate_counter_hpp__

#include "threadCommon.hpp"

namespace ApproximateCounter {
typedef struct __counter_t {
    int global;
    pthread_mutex_t glock;
    int local[NUMCPUS];
    pthread_mutex_t llock[NUMCPUS];
    int threshold;
} counter_t;

void init(counter_t *, int);
void update(counter_t *, int, int);
int get(counter_t *);
int total(counter_t *);
} // namespace ApproximateCounter

#endif