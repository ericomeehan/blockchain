// blockFactoryTest.c
//
// eric o meehan
// 2022-07-12
//
// unit tests for blockFactory

#include "blockFactory.h"

#define TEST_DATA_LENGTH 10

bool shouldEncodeBlock(void);
bool shouldDecodeBlock(void);
bool shouldCreateNewBlockWithMetadata(void);

bool shouldCreateNewBlockWithMetadata() {
	bool correctlyCreatedNewBlockWithMetadata = false;
	void * data = calloc(TEST_DATA_LENGTH, TEST_DATA_LENGTH);
	BlockMetadata * newBlockMetadata = newBlockWithMetadata(NULL, NULL, data, TEST_DATA_LENGTH);
	// check author
	// check signature
	// check timestamp format
	// check previous hash
	// check nonce / hash
	// 
