// blockDatabase.c
//
// eric o meehan
// 2022-07-06
//
// functions for indexing and querying the blockchain

unsigned long long calculateBlockWeight(Block * block) {
	unsigned long long blockWeight = block->headers.dataSize;
	unsigned long long numChildren = 0;
	Headers ** blockChildren = findChildrenOfBlock(block, &numChildren);
	for (int i = 0; i < numChildren; i++) {
		blockWeight += calculateBlockWeight(blockChildren[i]) * BLOCK_WEIGHT_MODIFIER;
	}
	return blockWeight;
}

Headers ** findChildrenOfBlock(Block *, unsigned long *) {
	// query for children hashes
	int numChildren = 0;
	Headers * children[numChildren];
	for (int i = 0; i < numChildren; i++) {
		children[i] = loadBlockHeadersFromFile();
	}
	return children;
}

