// blockchain.h
//
// eric o meehan
// 2022-06-24
//
// high level blockchain interface

#ifndef blockchain_h
#define blockchain_h

#include <stdio.h>
#include "block.h"
#include "chain.h"

SHA512Hash * calculateBlockHash(Block *);
void createNewBlock(Block *, RSAPublicKey *, void *, unsigned long);
void freeBlock(Block *);
void freeHeaders(Headers *);
Block * loadBlockFromFile(FILE *);
Headers * loadBlockHeadersFromFile(FILE *);
void saveBlockToFile(Block *, FILE *);
void saveBlockHeadersToFile(Headers *, FILE *);
void queryForAuthor(RSAPublicKey *, SHA512Hash *, unsigned long *);
void queryForChildren(SHA512Hash *, SHA512Hash *, unsigned long *);
void queryForDate(char *, SHA512Hash *, unsigned long *);
void queryForHash(SHA512Hash *, SHA512Hash *);
bool validateBlock(Block *);

#endif
