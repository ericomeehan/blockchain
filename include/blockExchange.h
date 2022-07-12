// blockExchange.h
//
// eric o meehan
//
// interface for sharing blocks with peers

#ifndef blockExchange_h
#define blockExchange_h

#include "block.h"
#include "blockValidator.h"

int sendBlock(BlockMetadata *, int);
BlockMetadata * receiveBlock(int, Validator *);

#endif // blockExchange_h //
