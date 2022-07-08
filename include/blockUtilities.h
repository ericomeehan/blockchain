// blockUtilities.h
//
// eric o meehan
// 2022-07-08
//
// interface for blockchain utility functions 

#ifndef blockUtilities_h
#define blockUtilities_h

#include "block.h"

int calculateDifficulty(unsigned long long);
void calculateTargetHash(int, SHA512Hash *);

#endif // blockUtilities_h //
