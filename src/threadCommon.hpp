#ifndef __thread_common_hpp__
#define __thread_common_hpp__

#include <pthread.h>
#include <sys/time.h>
#include <cstdlib>
#include <cstdio>

double getSeconds();

const int NUMCPUS = 12; // 6C12T force 48T

#endif