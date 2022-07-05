// block.h
//
// eric o meehan
// 2022-06-24
//
// required headers for blocks in the chain

#include <stdbool.h>

#define RSA_KEY_SIZE 1
#define SHA_512_HASH_SIZE 1
#define TIMESTAMP_SIZE 1
#define VALID_TIMESTAMP_REGEX abc123

typedef unsigned char Byte;
typedef unsigned char RSAPublicKey;
typedef unsigned char RSASignature;
typedef unsigned char SHA512Hash;

typedef struct Headers {
	RSAPublicKey author[RSA_KEY_SIZE];
	RSASignature signature[RSA_SIGNATURE_SIZE];
	SHA512Hash previousHash[SHA_512_HASH_SIZE];
	char timestamp[TIMESTAMP_SIZE];
	unsigned long dataSize;
	unsigned long nonce;
} Headers;

typedef struct Block {
	Headers headers;
	Byte data;
}

void signBlock(Block *, RSAPublicKey *);
void 
bool validateBlockDifficulty(Block *);
bool validateBlockSignature(Block *);
bool validateBlockTimestamp(Block *);
