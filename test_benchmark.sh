#!/bin/bash

if [[ $(uname -m) = "x86_64" ]]
then
	echo "Running on a x86 machine"
	TC="/usr/lib/x86_64-linux-gnu/libjemalloc.so.1"
	JE="/usr/lib/libtcmalloc_minimal.so.4.1.2"
else
	echo "Running on a ppc64el machine"
	TC="/usr/lib/libtcmalloc_minimal.so.4.2.6"
	JE="/usr/lib/powerpc64le-linux-gnu/libjemalloc.so.1"
fi

LIBC=""
ROUNDS=10000000
COUNT=5

for ALLOC in " " $JE $TC
do
	echo "Using allocator $ALLOC"
	for VARIABLE in 16 256 512 1024 4096 16384 65536
	do
		LD_PRELOAD=$ALLOC ./benchmark malloc    $COUNT $ROUNDS $VARIABLE
	done
done
