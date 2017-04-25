#!/bin/bash
for i in $(seq 1 1000)
do
	echo "PUT $1$i 78"|nc localhost 7777
done
