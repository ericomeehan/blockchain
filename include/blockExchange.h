// blockExchange.h
//
// eric o meehan
//
// interface for sharing blocks with peers

#ifndef blockExchange_h
#define blockExchange_h

void sendBlock(Block *, char *);
Block * receiveBlock(void);

#endif // blockExchange_h //
