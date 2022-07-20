# Blockchain

Eric O Meehan
2022-07-18

A blockchain implementation for arbitrary data and one-to-many parent-child relationships.

## Introduction
**This project is in early development**
This document describes the desired outcomes for the project, not its current state.  For up-to-date information, visit
https://www.eom.dev.

[video-journal-3a5bf0556fed](https://youtu.be/hZhg2RTO-eQ) provides an overview of blockchain concepts and a description of how
this project differs from the norm.

This blockchain is intended to store arbitrary data in a non-binary tree to produce an immutable, self-organizing ledgar of
network activity and content.  In this model, closely related data is linked upon creation, the impact of that data can be
measured precicely, and the contributor is rewarded for that impact directly.

## Products
The makefile in this repository can produce three outputs: a static library, a command line interface, and a block server
implementation.  All three files can be generated simply with:
```
make
```

### Library
A C library containing the core functionality for creating, validating, and sharing blocks.  This library can be used to create
custom blockchain applications.

```
make library
```

### CLI
A command line interface for performing basic blockchain operations.

```
usage: blockchain [create/validate/send] {args}
	create		takes arbitrary input data and a previous hash to mine a new block
		--input|-i)	get input from a specified file (default reads data from stdin)
		--output|-o)	write output to a specified file (default writes resulting block to stdout)

	validate	takes a block as input and outputs "valid" or "invalid" based on proof of work and author signature
		--input|-i)	get input from a specified file (default reads block from stdin)

	send		takes a block as input and outputs "accepted" or "refused {code}" based on the peer's response
		--input|-i)	get input from a specified file (default reads block from stdin)
		--host|-h)	ip address of destination host
```
```
make cli
```

### Server
A configurable server for collecting blocks.  This implementation reads blocks from network clients, performs a series of
validations on the block and its headers, and will either save or reject the saved block.

While a generic block server that accepts any valid block is possible, servers are intended to be customized to support a
separate application.  External, user validators can be specified in a configuration file for this server against which to
validate blocks and headers.  In doing so, the block server can be tailored to apply arbitrary scrutiny over which blocks are
relevant to the application and should be saved.

#### Validations
**Default Validations**
Every server on the network will validate every block it receives against the following requirements:
	proof-of-work		the hash of the received file satisfies the difficulty requirement for that file's size
	digital signature	bytes XXX-XXX are successfully used as an RSA key to validate bytes YYY-YYY as a signature for
					everything after byte ZZZ.
	trace-to-origin		a chain of hashes from previously validated blocks can be followed from this block to origin.

**User Validators**
Generally, block servers will only be interested in storing blocks that are relevant to the application they support.  As such,
the block server is able to execute external validations on blocks and headers at run time.  On launch, the block server will
read a list of unix domain socket addresses from EXTERNAL_BLOCK_VALIDATOR_ADDRESS_PATH and
EXTERNAL_HEADERS_VALIDATOR_ADDRESSES_PATH.  For each received block, the server will iterate over these lists, attempt to write
the given block or headers to the socket, and will read a boolean response indicating a pass or failure.  These validations can
examine header fields, block data, block lineage, or anything else needed to determine the data's relevance to the application.
The application developer is free to design and implement any validations beyond the defaults that they wish.

#### Compilation
```
compiler flag:					default value:				usage:
BLOCK_STORE_PATH				"/var/blockchain/blocks"		where block files are stored
EXTERNAL_BLOCK_VALIDATOR_ADDRESSES_PATH		"/etc/blockchain/blockValidators"	list unix socket addresses for block validators*
EXTERNAL_HEADERS_VALIDATOR_ADDRESSES_PATH	"/etc/blockchain/headerValidators"	list unix socket addresses for header validators*
MAX_BACKLOG					32					number of pending client connections before new connections are dropped
MAX_BLOCK_SIZE					0					largest size block this server will accept
PORT						12358					port the server is listening on
SHOULD_SAVE_FULL_BLOCK_DEFAULT			true					dictates the server's proclivity towards saving block files
THREAD_COUNT					4					number of threads that will be handling clients
```

```
make server {COMPILER_FLAGS}
```








