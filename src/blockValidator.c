// blockValidator.c
//
// eric o meehan
// 2022-07-06
//
// functions for validating blocks

bool hasValidAuthor(Block * block);
bool hasValidSignature(Block * block);
bool hasValidPreviousHash(Block * block);
bool hasValidTimestamp(Block * block);
bool hasValidDataSize(Block * block);
bool hasValidNonce(Block * block);

bool isValidBlock(Block * block) {
	return hasValidAuthor(block) && hasValidSignature(block) && hasValidPreviousHash(block) && hasValidTimestamp(block)
		&& hasValidDataSize(block) && hasValidNonce(block);
}


