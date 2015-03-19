clean:
	rm mystorage
	rm *.o
mystorage:
	gcc -g myhash.c -o myhash.o -c
	gcc -g -pthread server.c -o mystorage myhash.o
	rm myhash.o
