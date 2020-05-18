#include "threadCommon.hpp"

double getSeconds() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)((double)t.tv_sec + (double)t.tv_usec / 1e6);
}