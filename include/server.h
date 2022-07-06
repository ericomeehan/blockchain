// server.h
//
// eric o meehan
// 2022-07-06
//
// multithreaded, asynchronous server for conditionally accepting blocks

#ifndef server_h
#define server_h

#ifndef THREAD_COUND
#define THREAD_COUNT 1
#endif

#include "block.h"

void start(void);

#endif // server_h //
