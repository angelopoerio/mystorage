#include "myhash.h"

HashTable *initHash(int size) {
	int i;
	HashTable *hash = (HashTable *)malloc(sizeof(HashTable));

	if(hash == NULL)
		return NULL;

	hash->size = size;
	hash->table = (Entry **)malloc(sizeof(Entry *) * size);
	pthread_mutex_init(&hash->lock, NULL);
	for(i = 0;i < size;i++) {
		hash->table[i] = NULL;
	}
	
	return hash;
}

Entry *delEntry(char *key, Entry *head) {
  if (head == NULL)
    return NULL;

  if (strcmp(head->key, key) == 0) {
    Entry *tempNextP;
    tempNextP = head->next;
    free(head);
    return tempNextP;
  }

  head->next = delEntry(key, head->next);
  return head;
}

Entry *addEntry(Entry *list, char *key, char *value) {
      Entry *head = list;
      Entry *entry = list;

      if(list != NULL) {
	    while(entry->next != NULL) {
	    	/* no duplicate */
	        if(strcmp(entry->key, key) == 0) {
				free(entry->value);
				entry->value = (char *)malloc(strlen(value) + 1);
				strcpy(entry->value, value);
				return head;
			}
			
			entry = entry->next;
	    }

	    /* no duplicate */
	    if(strcmp(entry->key, key) == 0) {
	    	free(entry->value);
			entry->value = (char *)malloc(strlen(value) + 1);
			strcpy(entry->value, value);
	    }

	    else {
		    entry->next = (Entry *)malloc(sizeof(Entry));
		    entry->next->next = NULL;
		    entry->next->key = (char *)malloc(strlen(key) + 1);
		    strcpy(entry->next->key, key);
		    entry->next->value = (char *)malloc(strlen(value) + 1);
		    strcpy(entry->next->value, value);
		}
	    return head;
	}

	else {
		head = (Entry *)malloc(sizeof(Entry));
		head->value = (char *)malloc(strlen(value) + 1);
		head->key = (char *)malloc(strlen(key) + 1);
		strcpy(head->key, key);
		strcpy(head->value, value);
		head->next = NULL;
		return head;
	}
}

void put(HashTable **hash, char *key, char *value) {
	int index = getIndex(key);
	index = index % (*hash)->size;
	pthread_mutex_lock(&(*hash)->lock);
	Entry *entry = (*hash)->table[index];
	if(entry == NULL) {
		(*hash)->table[index] = addEntry(NULL, key, value);
	}

	else {
		(*hash)->table[index] = addEntry((*hash)->table[index], key, value);
	}

	pthread_mutex_unlock(&(*hash)->lock);
}

Entry *get(HashTable **hash, char *key) {
	int index = getIndex(key);
	index = index % (*hash)->size;
	pthread_mutex_lock(&(*hash)->lock);
	Entry *entry = (*hash)->table[index];

	if(entry == NULL)
		return NULL;

	while(strcmp(entry->key, key) != 0 && entry->next != NULL) {
		entry = entry->next;
	}

	pthread_mutex_unlock(&(*hash)->lock);
	
	if(entry != NULL && strcmp(entry->key, key) == 0)
		return entry;
	
	return NULL;
}

void del(HashTable **hash, char *key) {
	int index = getIndex(key);
	index = index % (*hash)->size;
	pthread_mutex_lock(&(*hash)->lock);
	(*hash)->table[index] = delEntry(key, (*hash)->table[index]);
	pthread_mutex_unlock(&(*hash)->lock);
}

/*  Bernstein hash */
int getIndex(char *key)
{
	unsigned char *p = key;
    int h = 0;
    int i;
    for (i = 0; i < strlen(key);i++)
    	h = 33 * h + p[i];
   return abs(h);
}
