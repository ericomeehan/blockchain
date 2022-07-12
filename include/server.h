// server.h
//
// eric o meehan
// 2022-07-06
//
// multithreaded, asynchronous server for conditionally accepting blocks

#ifndef server_h
#define server_h

#ifndef BLOCK_STORE_PATH
#define BLOCK_STORE_PATH "/var/blockchain/blocks"
#endif
#ifndef EXTERNAL_BLOCK_VALIDATOR_ADDRESSES_PATH
#define EXTERNAL_BLOCK_VALIDATOR_ADDRESSES_PATH "/etc/blockchain/blockValidators"
#endif
#ifndef EXTERNAL_HEADERS_VALIDATOR_ADDRESSES_PATH
#define EXTERNAL_HEADERS_VALIDATOR_ADDRESSES_PATH "/etc/blockchain/headerValidators"
#endif
#ifndef HEADERS_STORE_PATH
#define HEADERS_STORE_PATH "/var/blockchain/headers"
#endif
#ifndef MAX_BACKLOG
#define MAX_BACKLOG 32
#endif
#ifndef MAX_BLOCK_SIZE
#define MAX_BLOCK_SIZE 0
#endif
#ifndef PORT
#define PORT 12358
#endif
#ifndef SHOULD_SAVE_FULL_BLOCK_DEFAULT
#define SHOULD_SAVE_FULL_BLOCK_DEFAULT true
#endif
#ifndef THREAD_COUND
#define THREAD_COUNT 4
#endif

#endif // server_h //
