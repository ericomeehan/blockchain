// block.h
//
// eric o meehan
// 2022-06-24
//
// defines a valid block 

#ifndef block_h
#define block_h

#define BLOCK_HEADERS_SIZE
#define DATA_SIZE_LENGTH 8
#define PEM_RSA_PUBLIC_KEY_SIZE 801
#define RSA_PRIVATE_KEY_SIZE 4096
#define SHA_512_HASH_SIZE 130
#define TIMESTAMP_SIZE 30
#define VALID_TIMESTAMP_REGEX abc123

#include <openssl/evp.h>
#include <stdbool.h>
#include <time.h>

typedef unsigned char Block;
typedef unsigned char Byte;
typedef char RSAPublicKey;
typedef char RSASignature;
typedef char SHA512Hash;

typedef struct EncodedBlockHeaders {
	RSAPublicKey author[PEM_RSA_PUBLIC_KEY_SIZE];
	RSASignature signature[SHA_512_HASH_SIZE];
	SHA512Hash previousHash[SHA_512_HASH_SIZE];
	char timestamp[TIMESTAMP_SIZE];
	char dataSize[DATA_SIZE_LENGTH];
	unsigned long long nonce;
} EncodedBlockHeaders;

typedef struct EncodedBlock {
	EncodedBlockHeaders headers;
	Byte data;
} EncodedBlock;

typedef struct BlockMetadata {
	EVP_PKEY * authorKey;
	void * block;
	unsigned long long blockDifficulty;
	bool blockIsEncoded;
	unsigned long long blockSize;
	SHA512Hash hash[SHA_512_HASH_SIZE];
	time_t timestamp;
}

#endif // block_h //
