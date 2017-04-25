#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include "myhash.h"

#define SERVER_PORT 7777 /* change this according to your needs */
#define TABLE_SIZE 10000 /* buckets entries */
#define INVALID_CMD(fd) send(fd,"Invalid cmd\n", 12, 0);

static HashTable *hash;

void *connection_handler(void *socket);
void mytrim(char *str);
int parseCmd(int fd, char *cmd);
void startServer(void);