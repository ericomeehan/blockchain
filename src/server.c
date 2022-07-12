// server.c
//
// eric o meehan
// 2022-07-09
//
// server for accepting blocks from peers

#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "block.h"
#include "blockExchange.h"
#include "blockFactory.h"
#include "blockValidator.h"
#include "server.h"

typedef struct Job {
	struct Job * next;
	int client;
} Job;
typedef struct ThreadPool {
	bool isActive;
	Job * nextJob;
	Job * lastJob;
	pthread_t * pool;
	pthread_mutex_t lock;
	pthread_cond_t signal;
	Validator * headerValidators;
	Validator * blockValidators;
} ThreadPool;

void conditionallySaveBlock(BlockMetadata *, Validator *);
void handleConnections(int);
void * handleClients(void *);
void loadDefaultValidators(ThreadPool *);
void loadExternalValidators(ThreadPool *);
bool validateBlockSize(BlockMetadata *, char *);
bool validateIdempotency(BlockMetadata *, char *);
bool validateExternally(BlockMetadata *, char *);

int main(int argc, char ** argv) {
	int sock;
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		// error
	}
	if ((bind(sock, (struct sockaddr *)&address, sizeof(address))) < 0) {
		// error
	}
	if ((listen(sock, MAX_BACKLOG)) < 0) {
		// error
	}
	ThreadPool threadPool;
	threadPool.headerValidators = (Validator *) malloc(sizeof(Validator));
	threadPool.blockValidators = (Validator *) malloc(sizeof(Validator));
	loadDefaultValidators(&threadPool);
	loadExternalValidators(&threadPool);
	pthread_mutex_lock(&threadPool.lock);
	threadPool.pool = malloc(sizeof(pthread_t[THREAD_COUNT]));
	for (int i = 0; i < THREAD_COUNT; i++) {
		pthread_create(&threadPool.pool[i], NULL, handleClients, &threadPool);
	}
	pthread_mutex_unlock(&threadPool.lock);
	struct sockaddr * socketAddress = (struct sockaddr *) &address;
	socklen_t addressLength = (socklen_t) sizeof(address);
	while (true) {
		Job * job = (Job *) malloc(sizeof(Job));
		job->client = accept(sock, socketAddress, &addressLength);
		pthread_mutex_lock(&threadPool.lock);
		threadPool.lastJob->next = job;
		threadPool.lastJob = job;
		pthread_cond_signal(&threadPool.signal);
		pthread_mutex_unlock(&threadPool.lock);
	}
	free(threadPool.pool);
}

void * handleClients(void * pool) {
	ThreadPool * threadPool = (ThreadPool *) pool;
	while (threadPool->isActive) {
		pthread_mutex_lock(&threadPool->lock);
		pthread_cond_wait(&threadPool->signal, &threadPool->lock);
		Job * job = threadPool->nextJob;
		threadPool->nextJob = job->next;
		pthread_mutex_unlock(&threadPool->lock);
		BlockMetadata * blockMetadata = receiveBlock(job->client, threadPool->headerValidators);
		conditionallySaveBlock(blockMetadata, threadPool->blockValidators);
		free(job);
	}
	return NULL;
}

void conditionallySaveBlock(BlockMetadata * blockMetadata, Validator * validators) {
	if (blockMetadata->blockIsValid) {
		char filePath[PATH_MAX];
		strcpy(HEADERS_STORE_PATH, filePath);
		strcat(filePath, blockMetadata->hash);
		FILE * file = fopen(filePath, "w");
		if (blockMetadata->blockIsEncoded) { decodeBlock(blockMetadata); }
		fwrite(blockMetadata->block, BLOCK_HEADERS_SIZE, 1, file);
		fclose(file);
		bool shouldSaveFullBlock = SHOULD_SAVE_FULL_BLOCK_DEFAULT;
		Validator * validator = validators;
		while (validator && shouldSaveFullBlock == SHOULD_SAVE_FULL_BLOCK_DEFAULT) {
			shouldSaveFullBlock = validator->validationFunction(blockMetadata, validator->address);
			validator = validator->next;
		}
		if (shouldSaveFullBlock) {
			memset(filePath, 0, PATH_MAX);
			strcpy(BLOCK_STORE_PATH, filePath);
			strcat(filePath, blockMetadata->hash);
			file = fopen(filePath, "w");
			fwrite(blockMetadata->block, blockMetadata->blockSize, 1, file);
			fclose(file);
		}
	}
}

void loadDefaultValidators(ThreadPool * threadPool) {
	Validator * validator = threadPool->blockValidators;
	while (validator->next) { validator = validator->next; }
	validator->next = (Validator *) malloc(sizeof(Validator));
	validator = validator->next;
	validator->validationFunction = validateBlockSize;
	validator->next = (Validator *) malloc(sizeof(Validator));
	validator = validator->next;
	validator->validationFunction = validateIdempotency;
}

void loadExternalValidators(ThreadPool * threadPool) {
	char buffer[PATH_MAX] = {0};
	FILE * file = fopen(EXTERNAL_HEADERS_VALIDATOR_ADDRESSES_PATH, "r");
	Validator * validator = threadPool->headerValidators;
	while (fgets(buffer, PATH_MAX, file)) {
		while (validator->next) { validator = validator->next; }
		validator->next = calloc(sizeof(Validator), 1);
		validator = validator->next;
		validator->validationFunction = validateExternally;
		memcpy(validator->address, buffer, strlen(buffer));
		memset(buffer, 0, PATH_MAX);
	}
	fclose(file);
	file = fopen(EXTERNAL_BLOCK_VALIDATOR_ADDRESSES_PATH, "r");
	validator = threadPool->blockValidators;
	while (fgets(buffer, PATH_MAX, file)) {
		while (validator->next) { validator = validator->next; }
		validator->next = calloc(sizeof(Validator), 1);
		validator = validator->next;
		validator->validationFunction = validateExternally;
		memcpy(validator->address, buffer, strlen(buffer));
		memset(buffer, 0, PATH_MAX);
	}
	fclose(file);
}

bool validateBlockSize(BlockMetadata * blockMetadata, char * ADDRESS_NOT_USED) {
	return blockMetadata->blockSize <= MAX_BLOCK_SIZE;
}

bool validateExternally(BlockMetadata * blockMetadata, char * address) {
	int sock = socket(AF_UNIX, SOCK_STREAM, 0);
	// error
	struct sockaddr_un socketAddress;
	bool response = false;
	memset(&socketAddress, 0, sizeof(struct sockaddr_un));
	socketAddress.sun_family = AF_UNIX;
	strncpy(socketAddress.sun_path, address, strlen(address));
	connect(sock, (struct sockaddr *) &socketAddress, sizeof(struct sockaddr_un));
	write(sock, blockMetadata->block, blockMetadata->blockSize);
	read(sock, &response, sizeof(bool));
	close(sock);
	return response;
}

bool validateIdempotency(BlockMetadata * blockMetadata, char * ADDRESS_NOT_USED) {
	char blockPath[PATH_MAX] = {0};
	strcat(blockPath, HEADERS_STORE_PATH);
	strcat(blockPath, "/");
	strcat(blockPath, blockMetadata->hash);
	FILE * f;
	if (f = fopen(blockPath, "r")) {
		fclose(f);
		return false;
	}
	return true;
}
