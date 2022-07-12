// blockFactory.c
//
// eric o meehan
// 2022-07-06
//
// functions for operating on blocks

#include "blockFactory.h"
#include "blockUtilities.h"
#include "util.h"

#define NONCE_ADDRESS_OFFSET 0

void computeNonce(BlockMetadata *);

BlockMetadata * newBlockMetadata(RSAPublicKey * key, SHA512Hash * previousHash,
		void * data, unsigned long long dataSize) {
	BlockMetadata * blockMetadata = (BlockMetadata *) malloc(sizeof(BlockMetadata));
	blockMetadata->block = calloc(BLOCK_HEADERS_SIZE + dataSize);
	EncodedBlock * encodedBlock = (EncodedBlock *) blockMetadata->block;
	blockMetadata->blockIsEncoded = true;

	loadRsaKey(blockMetadata->authorKey, key);
	memcpy(key, encodedBlock->author, 1, RSA_PUBLIC_KEY_SIZE);

	createRSASignature(blockMetadata->authorKey, newBlock->signature, data, dataSize);

	memcpy(previousHash, encodedBlock->previousHash, 1, SHA_512_HASH_SIZE);

	time(&blockMetadata->timestamp);
	struct tm *ptm = gmtime(&now);
	sprintf(encodedBlock->timestamp, "%s", asctime(ptm));

	blockMetadata->blockSize = BLOCK_HEADERS_SIZE + dataSize;
	sprintf(encodedBlock->dataSize, "%llu", dataSize);

	memcpy(&encodedBlock->data, data, dataSize);

	computeNonce(blockMetadata);

	return blockMetadata;
}

void encodeBlock(BlockMetadata * blockMetadata) {
	for (int i = 0; i < NUM_ENCODED_BYTES; i++) {
		((unsigned char *) blockMetadata->block)[ENCODED_BYTES[i]] = TERMINAL_CHARACTER;
	}
	blockMetadata->blockIsEncoded = true;
}

void decodeBlock(BlockMetadata * blockMetadata) {
	for (int i = 0; i < NUM_ENCODED_BYTES; i++) {
		block[ENCODED_BYTES[i]] = NEWLINE_CHARACTER;
	}
	blockMetadata->blockIsEncoded = false;
}

void freeBlockMetadata(BlockMetadata * blockMetadata) {
	EVP_PKEY_free(blockMetadata->authorKey);
	free(blockMetadata->block);
	free(blockMetadata);
}

void computeNonce(BlockMetadata * blockMetadata) {
	bool blockWasEncoded = blockMetadata->blockIsEncoded;
	if (blockWasEncoded) { decodeBlock(blockMetadata); }
	while (!proofOfWork(blockMetadata)) {
		(unsigned long long) blockMetadata->block[NONCE_ADDRESS_OFFSET] += 1;
	}
	if (blockWasEncoded) { encodeBlock(blockMetadata); }
}
