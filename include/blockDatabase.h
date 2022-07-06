// blockDatabase.h
//
// eric o meehan
// 2022-07-06
//
// interface for indexing and querying the blockchain

#ifndef blockIndex_h
#define blockIndex_h

#ifndef BLOCKCHAIN_DATABASE_PATH
#define BLOCKCHAIN_DATABASE_PATH /var/lib/blockchain.sql

#define BLOCKCHAIN_DATABASE_SCHEMA TEMP

#define CHILD_WEIGHT_MODIFIER .10

#include "block.h"

void addBlockToDatabase(Block *);
Headers ** findChildrenOfBlock(Block *, unsigned long *);
bool blockExistsInDatabase(Block *);
void traceBlockToOrigin(Block *);
unsigned long long calculateBlockWeight(Block *);
void queryBlockchainDatabase(char *);

#endif
