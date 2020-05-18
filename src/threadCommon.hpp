#ifndef __thread_common_hpp__
#define __thread_common_hpp__

#include <pthread.h>
#include <sys/time.h>
#include <cstdlib>

double getSeconds();

const int NUMCPUS = 12; // 6C12T

#endif