# mystorage
In memory key/value storage. Multithread and TCP/IP based service

# How to build
Just use: make mystorage

# Supported commands

It listens on the TCP port 7777 for the following commands:

PUT key value - put the key/value in the dataset

GET key - get the key

DEL key - delete the key

LIST - list all the keys

QUIT - quit the current session

# Populate with a lot of key/value pairs

Look at stress.py for an useful example. Do not open a new connection to insert a new key/value pair,
just use one and issue the command many times in the current session. This is to avoid the overhead of
opening too many connections that will cause the server to block. Use the same strategy for the other commands 
(GET/LIST/DEL) 


# Author & Notes
  Written by Angelo Poerio <angelo.poerio@gmail.com>

  Please note this piece of software is not production ready! Use it at your own risk!
