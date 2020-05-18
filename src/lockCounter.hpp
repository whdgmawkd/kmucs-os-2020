#ifndef __lock_counter_hpp__
#define __lock_counter_hpp__

#include "threadCommon.hpp"

namespace LockCounter {
typedef struct __counter_t {
    int value;
    pthread_mutex_t lock;
} counter_t;

void init(counter_t *);
void increment(counter_t *);
void decrement(counter_t *);
int get(counter_t *);
} // namespace LockCounter

#endif