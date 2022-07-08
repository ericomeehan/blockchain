// blockUtilities.c
//
// eric o meehan
// 2022-07-08
//
// functions for blockchain utilities

#include <math.h>

#include "blockUtilities.h"
#include "util.h"

#define SHA_512_HASH_BYTES 512

int calculateDifficulty(unsigned long long blockSize) {
	return SHA_512_HASH_BYTES - pow(log(sqrt(blockSize)), 2) + 1;
}

void calculateTargetHash(int difficulty, SHA512Hash * hash) {
	arbitrarilyPrecisePowerOfTwo(SHA_512_HASH_BYTES - difficulty, hash);
}
