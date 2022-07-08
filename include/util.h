// util.h
//
// eric o meehan
// 2022-07-08
//
// bespoke utilities for interfacing with external libraries

#ifndef util_h
#define util_h

#include "block.h"

int compareSHA512Hashes(unsigned char *, unsigned char *);
void createRSASignature(EVP_PKEY *, unsigned char *, void *, unsigned long long);
void generateSHA512Hash(unsigned char *, unsigned char *, unsigned long long);
void loadRsaKey(EVP_PKEY *, unsigned char *);

#endif // util_h //
