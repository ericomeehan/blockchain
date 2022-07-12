// blockUtilities.c
//
// eric o meehan
// 2022-07-08
//
// functions for blockchain utilities

#include <math.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "blockFactory.h"
#include "blockUtilities.h"
#include "util.h"

#define SHA_512_HASH_BYTES 512

int calculateDifficulty(unsigned long long blockSize) {
	return SHA_512_HASH_BYTES - pow(log(sqrt(blockSize)), 2) + 1;
}

void calculateTargetHash(int difficulty, SHA512Hash * hash) {
	arbitrarilyPrecisePowerOfTwo(SHA_512_HASH_BYTES - difficulty, hash);
}

int externalValidation(BlockMetadata * blockMetadata, char * path) {
	int sock = socket(AF_UNIX, SOCK_STREAM, 0);
	// check for errors
	struct sockaddr_un server;
	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, path);
	if (connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) {
		// errors
	}
	bool blockWasEncoded = blockMetadata->blockIsEncoded;
	if (blockWasEncoded) { decodeBlock(blockMetadata); }
	write(sock, blockMetadata->block, blockMetadata->blockSize);
	int response;
	read(sock, &response, 4);
	close(sock);
	return response;
}
