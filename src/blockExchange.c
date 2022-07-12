// blockExchange.c
//
// eric o meehan
//
// protocol for sharing blocks with peers

#include <unistd.h>

#include "blockExchange.h"
#include "blockFactory.h"

#define RESPONSE_SIZE 1

int sendBlock(BlockMetadata * blockMetadata, int socket) {
	bool blockWasEncoded = blockMetadata->blockIsEncoded;
	if (blockWasEncoded) { decodeBlock(blockMetadata); }
	int response = 0;
	write(socket, blockMetadata->block, BLOCK_HEADERS_SIZE);
	read(socket, &response, RESPONSE_SIZE);
	if (response) {
		response = 0;
		write(socket, blockMetadata->block, blockMetadata->blockSize);
		read(socket, &response, RESPONSE_SIZE);
	}
	close(socket);
	if (blockWasEncoded) { encodeBlock(blockMetadata); }
	return response;
}

BlockMetadata * receiveBlock(int socket, Validator * validators) {
	BlockMetadata * blockMetadata = (BlockMetadata *) malloc(sizeof(BlockMetadata));
	blockMetadata->block = (EncodedBlock *) calloc(BLOCK_HEADERS_SIZE + 1, 1);
	read(socket, blockMetadata->block, BLOCK_HEADERS_SIZE);
	blockMetadata->blockIsEncoded = false;
	decodeBlock(blockMetadata);
	blockMetadata->blockSize = BLOCK_HEADERS_SIZE + atoll(((EncodedBlock *) blockMetadata->block)->headers.dataSize);
	bool response = true;
	Validator * validator = validators;
	while (validator && response) {
		response = validator->validationFunction(blockMetadata, validator->address);
		validator = validator->next;
	}
	write(socket, &response, RESPONSE_SIZE);
	if (response) {
		free(blockMetadata->block);
		blockMetadata->block = malloc(blockMetadata->blockSize);
		read(socket, blockMetadata->block, blockMetadata->blockSize);
		response = proofOfAuthor(blockMetadata) && proofOfWork(blockMetadata);
		write(socket, &response, RESPONSE_SIZE);
	}
	close(socket);
	blockMetadata->blockIsValid = response;
	return blockMetadata;
}
