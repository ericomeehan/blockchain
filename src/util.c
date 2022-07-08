// util.c
//
// eric o meehan
// 2022-07-08
//
// bespoke utilities for interfacing with external libraries

#include <openssl/bio.h>

#include "util.h"

void arbitrarilyPrecisePowerOfTwo(int power, unsigned char * hexidecimalOutput) {
	BN_CTX * ctx = BN_CTX_new();

	BIGNUM * base = BN_new();
	BN_dec2bn(&base, "2");
	
	BIGNUM * exponent = BN_new();
	char decimalPower[8];
	sprintf(decimalPower, "%df", power);
	BN_dec2bn(&exponent, decimalPower);

	BIGNUM * result = BN_new();
	BN_exp(result, base, exponent, ctx);

	BIGNUM * hexidecimal = BN_new();
	char * tmp = BN_bn2hex(result);
	sprintf(hexidecimalOutput, "%0129s", tmp);
	
	free(tmp);
	BN_free(base);
	BN_free(exponent);
	BN_free(result);
	BN_free(hexidecimal);
	BN_CTX_free(ctx);
}


void createRSASignature(EVP_PKEY * key, unsigned char * digest, void * data, unsigned long long dataSize) {
	EVP_MD_CTX * ctx = EVP_MD_CTX_new();
	if ((EVP_DigestSignInit(ctx, NULL, EVP_sha512(), NULL, rsaKey)) <= 0) {
		// error
	}
	if (EVP_DigestSignUpdate(ctx, &block->data, block->headers.dataSize) <=0) {
		// error
	}
	size_t len;
	if (EVP_DigestSignFinal(ctx, (unsigned char *) digest, &len) <= 0) {
		// error
	}
	EVP_MD_CTX_free(ctx);
}

void generateSHA512Hash(unsigned char * digest, unsigned char * data, unsigned long long dataSize) {
	EVP_MD_CTX * ctx = EVP_MD_CTX_new();
	if (EVP_DigestInit_ex(ctx, EVP_sha512(), NULL) <= 0) {
		// error
	}
	if (EVP_DigestUpdate(ctx, data, dataSize) <= 0) {
		// error
	}
	unsigned int size = 64;
	if (EVP_DigestFinal(ctx, digest, &size) <= 0) {
		// error
	}
	EVP_MD_CTX_free(ctx);
}

void loadRSAKey(EVP_PKEY * loadedKey, unsigned char * unloadedKey) {
	// ???
}
