#!/bin/bash

TC="/usr/lib/libtcmalloc_minimal.so.4.2.6"
JE="/usr/lib/powerpc64le-linux-gnu/libjemalloc.so.1"
LIBC=""
ROUNDS=1000000
COUNT=5

for ALLOC in " " $JE $TC
do
	echo "Using allocator $ALLOC"
	for VARIABLE in 16 256 512 1024 4096 16384 65536
	do
		LD_PRELOAD=$ALLOC ./benchmark malloc    $COUNT $ROUNDS $VARIABLE
	done
done
