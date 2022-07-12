// blockFactory.c
//
// eric o meehan
// 2022-07-06
//
// functions for operating on blocks

#include <string.h>

#include "blockFactory.h"
#include "blockUtilities.h"
#include "blockValidator.h"
#include "util.h"

#define NONCE_ADDRESS_OFFSET 0

void computeNonce(BlockMetadata *);

BlockMetadata * newBlockMetadata(RSAPublicKey * key, SHA512Hash * previousHash,
		void * data, unsigned long long dataSize) {
	BlockMetadata * blockMetadata = (BlockMetadata *) malloc(sizeof(BlockMetadata));
	blockMetadata->block = calloc(BLOCK_HEADERS_SIZE + dataSize, 1);
	EncodedBlock * encodedBlock = (EncodedBlock *) blockMetadata->block;
	blockMetadata->blockIsEncoded = true;

	loadRSAKey(blockMetadata->authorKey, key);
	memcpy(key, encodedBlock->headers.author, PEM_RSA_PUBLIC_KEY_SIZE);

	createRSASignature(blockMetadata->authorKey, encodedBlock->headers.signature, data, dataSize);

	memcpy(previousHash, encodedBlock->headers.previousHash, SHA_512_HASH_SIZE);

	time(&blockMetadata->timestamp);
	time_t rawTime;
	time(&rawTime);
	struct tm *ptm;
	ptm = gmtime(&rawTime);
	sprintf(encodedBlock->headers.timestamp, "%s", asctime(ptm));

	blockMetadata->blockSize = BLOCK_HEADERS_SIZE + dataSize;
	sprintf(encodedBlock->headers.dataSize, "%llu", dataSize);

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
		((unsigned char *) blockMetadata->block)[ENCODED_BYTES[i]] = NEWLINE_CHARACTER;
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
	unsigned long long * nonce = (unsigned long long *) blockMetadata->block + NONCE_ADDRESS_OFFSET;
	while (!proofOfWork(blockMetadata)) {
		*nonce += 1;
	}
	if (blockWasEncoded) { encodeBlock(blockMetadata); }
}
