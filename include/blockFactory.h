// blockFactory.h
//
// eric o meehan
// 2022-07-06
//
// interface for operating on blocks

#ifndef blockFactory_h
#define blockFactory_h

#define NUM_ENCODED_BYTES 4
#define ENCODED_BYTES {1, 2, 3, 4}
#define NEWLINE_CHARACTER '\n'
#define TERMINAL_CHARACTER = '\0'

#include "block.h"

BlockMetadata * getBlockMetadata(RSAPublicKey *, SHA512Hash *, void *, unsigned long long);

void encodeBlock(BlockMetadata *);
void decodeBlock(BlockMetadata *);

void freeBlockMetadata(BlockMetadata *);

#endif // blockFactory_h //
