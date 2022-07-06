// blockFactory.h
//
// eric o meehan
// 2022-07-06
//
// interface for operating on blocks

#ifndef blockFactory_h
#define blockFactory_h

#include "block.h"

Block * createBlock(RSAPrivateKey *, SHA512Hash *, void *, unsigned long);
Block * loadBlockFromFile(char *);
void saveBlockToFile(Block *, char *);
void calculateBlockHash(Block *, SHA512Hash *);
void freeBlock(Block *);

#endif // blockFactory_h //
