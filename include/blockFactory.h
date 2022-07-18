// blockFactory.h
//
// eric o meehan
// 2022-07-06
//
// interface for operating on blocks

#ifndef blockFactory_h
#define blockFactory_h

#define NUM_ENCODED_BYTES 4
#define NEWLINE_CHARACTER '\n'
#define TERMINAL_CHARACTER '\0'

#include "block.h"

const int ENCODED_BYTES[4] = {1, 2, 3, 4};

BlockMetadata * newBlockWithMetadata(RSAPublicKey *, SHA512Hash *, void *, unsigned long long);

void encodeBlock(BlockMetadata *);
void decodeBlock(BlockMetadata *);

void freeBlockMetadata(BlockMetadata *);

#endif // blockFactory_h //
