#include "block.h"

// define valid block
// define invalid block
// define RSA public key

bool shouldValidateValidBlock(void) {
	return validateBlockDifficulty(&VALID_BLOCK)
		&& validateBlockSignature(&VALID_BLOCK);
}

bool shouldNotValidateInvalidBlock(void) {
	return !validateBlockDifficulty(&INVALID_BLOCK)
		&& !validateBlockSignature(&INVALID_BLOCK);
}

bool shouldCreateValidNewBlock(void) {
	Block newBlock;
	// copy author
	signBlock(&newBlock, &RSA_KEY);
	while (!validateBlockDifficulty(&newBlock)) {
		newBlock.nonce += 1;
	}
	return validateBlockDifficulty(&newBlock)
		&& validateBlockSignature(&newBlock);
}
