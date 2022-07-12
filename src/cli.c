// cli.c
//
// eric o meehan
// 2022-07-06
//
// command line interface for the blockchain

#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "blockExchange.h"
#include "blockFactory.h"
#include "blockValidator.h"

#define DEFAULT_PORT 12358

int help(void);
unsigned long long readFromStdin(void *);

int main(int argc, char ** argv) {
	BlockMetadata blockMetadata;
	if (argc < 2) { return help(); }
	char * mode = argv[1];
	if (strcmp(mode, "create") == 0) { 
		if (argc != 4) { return help(); }

	}
	else if (strcmp(mode, "extract") == 0) {
		blockMetadata.blockSize = readFromStdin(blockMetadata.block);
		encodeBlock(&blockMetadata);
		fwrite(
			&((EncodedBlock *) blockMetadata.block)->data,
			atoll(((EncodedBlock *) blockMetadata.block)->headers.dataSize),
			1,
			stdout
		);
		return EXIT_SUCCESS;
	}
	else if (strcmp(mode, "share") == 0) {
		if (argc < 3) { return help(); }
		blockMetadata.blockSize = readFromStdin(blockMetadata.block);
		encodeBlock(&blockMetadata);
		char * address = argv[2];
		int port = argc > 3 ? atoi(argv[3]) : DEFAULT_PORT;
		int sock;
		struct sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(port);
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
			// error
		}
		if ((connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress))) <=0) {
			// error
		}
		return sendBlock(&blockMetadata, sock);
	}

}

unsigned long long readFromStdin(void * data) {
	FILE * f = fopen("/dev/stdin", "r");
	fseek(f, 0L, SEEK_END);
	unsigned long long size = ftell(f);
	fseek(f, 0L, SEEK_SET);
	data = malloc(size + 1);
	fread(data, size, 1, f);
	return size;
}
