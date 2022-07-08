// blockValidator.c
//
// eric o meehan
// 2022-07-06
//
// functions for validating blocks

#include "blockFactory.h"
#include "blockValidator.h"
#include "util.h"

bool proofOfAuthor(BlockMetaddata * blockMetadata) {
	bool blockWasEncoded = blockMetadata->blockIsEncoded;
	if (!blockWasEncoded) { encodeBlock(block); }
	bool signatureIsValid = isValidRSASignature(blockMetadata->authorKey, blockMetadata->block, blockMetadata->blockSize);
	if (!blockWasEncoded) { decodeBlock(blockMetadata); }
	return signatureIsValid;
}

bool proofOfWork(BlockMetadata * blockMetadata) {
	bool blockWasEncoded = blockMetadata->blockIsEncoded;
	if (blockWasEncoded) { decodeBlock(blockMetadata); }
	unsigned char targetHash[SHA_512_HASH_SIZE];
	calculateTargetHash(calculateDifficulty(blockMetadata->blockSize), targetHash);
	return compareSHA512Hashes(blockMetadata->hash, targetHash) == -1;
}
