#include "server.h"

void *connection_handler(void *socket)
{
	int connfd = *(int *)socket, ret;
	char msg[500];
	int retval;

	do {
    	bzero(msg, 500);
    	retval = read(connfd, msg, 500);
		if(retval <= 0)
		    break;
    } while(parseCmd(connfd, msg));
    	
    close(connfd);
    pthread_exit(0);
}

void mytrim(char *str) {
	int i;

	for(i = 0;i < strlen(str);i++) {
		if(str[i] == '\r' || str[i] == '\n')
		{
			str[i] = '\0';
			break;
		}
	}
}

int parseCmd(int fd, char *cmd) {
	char op[20], key[100], value[370];
	char *res;
	int i;
	Entry *entry;
	bzero(op, 20);
	bzero(key, 100);
	bzero(value, 370);
	sscanf(cmd, "%s %s %s",op,key,value);
	mytrim(value);
	mytrim(op);
	mytrim(key);
	
	if(strcasecmp(op,"QUIT") == 0)
		return 0;

	if(strcasecmp(op, "PUT") == 0) {
		if(strlen(key) == 0 || strlen(value) == 0) {
			INVALID_CMD(fd)
		}

		else {
			put(&hash, key, value);
		}
	}

	if(strcasecmp(op, "GET") == 0) {
		if(strlen(key) == 0) {
			INVALID_CMD(fd)
		}

		else {
			get(&hash, key);

			if(entry == NULL) {
				send(fd, "doesn't exist\n",14,0);
			}

			else {
				send(fd, entry->value, strlen(entry->value), 0);
			}
		}
	}

	if(strcasecmp(op, "DEL") == 0) {
		if(strlen(key) == 0) {
			INVALID_CMD(fd)
		}

		else {
			del(&hash, key);
		}
	}

	if(strcasecmp(op, "LIST") == 0) {
		pthread_mutex_lock(&hash->lock);
		for(i = 0;i < hash->size;i++) {
			if(hash->table[i] != NULL) {
				entry = hash->table[i];
				while(entry != NULL) {
					res = (char *)malloc(strlen(entry->key) + strlen(entry->value) + 10);
					sprintf(res, "%s:%s\n",entry->key, entry->value);
					send(fd,res,strlen(res),0);
					free(res);
					entry = entry->next;
				}
			}
		}
		pthread_mutex_unlock(&hash->lock);
	}

	return 1;
}

void startServer(void) {
	int listenfd,connfd, ret;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t clilen;
    int flags = 1;

    listenfd=socket(AF_INET,SOCK_STREAM,0);

    if(listenfd < 0) {
    	printf("socket() failed\n");
    	return;
    }

    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(int));

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(SERVER_PORT);
    if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0) {
    	printf("bind() failed\n");
    	return;
    }

    listen(listenfd, 10000);
    printf("mystorage running on %d/TCP\n", SERVER_PORT);
    printf("%s", "Author: Angelo Poerio <angelo.poerio@gmail.com>\n");

    for(;;)
    {
    	clilen=sizeof(cliaddr);
        connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
        pthread_t t_id;

        if(pthread_create(&t_id,NULL,connection_handler, (void*)&connfd) < 0)
        {
            printf("could not start thread");
            return;
        }

    }
} 

int main(int argc,char *argv[]) {
	int pid;
	hash = initHash(TABLE_SIZE);

	if(hash == NULL) {
		printf("Couldn't malloc() memory\n");
		return 0;
	}
	
	pid = fork();

	if(pid > 0) {
		return; /* parent */
	}

	else {
		startServer();
	}
}
