// blockFactory.c
//
// eric o meehan
// 2022-07-06
//
// functions for operating on blocks

#include "blockFactory.h"

Block * allocateBlockMemory(unsigned long);
void setPreviousHash(Block *, SHA512Hash *);
void writeDataToBlock(Block *, void *, unsigned long);
void timestampBlock(Block *);
void signBlock(Block *, RSAPrivateKey *);
void calculateNonce(void);

Block * createBlock(RSAPrivateKey * rsaKey, SHA512Hash * previousHash, void * data, unsigned long dataSize) {
	Block * newBlock = allocateBlockSpace(dataSize);
	setPreviousHash(newBlock, previousHash);
	writeDataToBlock(newBlock, data, dataSize);
	timestampBlock(newBlock);
	signBlock(newBlock, rsaKey);
	calculateNonce(newBlock);
	return newBlock;
}

Block * loadBlockFromFile(char *) {
	// do something
}

void saveBlockToFile(Block * block, char * path) {
	// do something
}

void calculateBlockHash(Block *, SHA512Hash *);

void freeBlock(Block * block) {
	free(block);
}

Block * allocateBlockMemory(unsigned long dataSize) {
	return (Block *) malloc(BLOCK_HEADER_SIZE + dataSize);
}

void setPreviousHash(Block * block, SHA512Hash * hash) {
	memcpy(hash, &block->headers.previousHash, SHA_512_HASH_SIZE);
}

void writeDataToBlock(Block * block, void * data, unsigned long dataSize) {
	memcpy(data, &block->data, dataSize);
}

void timestampBlock(Block * block) {
	// ???
}

void signBlock(Block * block, RSAPrivateKey rsaKey) {
	// openSsl
}

void calculateNonce(Block * block) {
	while(true) {
		block->headers.nonce += 1;
	}
}
