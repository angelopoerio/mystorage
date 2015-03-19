#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct Entry {
	char *key;
	char *value;
	struct Entry *next;
};

typedef struct Entry Entry;

struct HashTable {
	int size;
	pthread_mutex_t lock;
	struct Entry **table;
};

typedef struct HashTable HashTable;

HashTable *initHash(int size);
Entry *addEntry(Entry *list, char *key, char *value);
void put(HashTable **hash, char *key, char *value);
Entry *get(HashTable **hash, char *key);
void del(HashTable **hash, char *key);
int getIndex(char *key);