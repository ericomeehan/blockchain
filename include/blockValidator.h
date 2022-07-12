// blockValidator.h
//
// eric o meehan
// 2022-07-06
//
// interface for validating blocks

#ifndef blockValidator_h
#define blockValidator_h

#include "block.h"

typedef struct Validator {
	struct Validator * next;
	bool (* validationFunction)(BlockMetadata *, char *);
	char address[PATH_MAX];
} Validator;

bool proofOfAuthor(BlockMetadata *);
bool proofOfWork(BlockMetadata *);

#endif // blockValidator_h //
