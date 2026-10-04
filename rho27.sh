#!/usr/bin/bash

rm -f /tmp/*.txt
NL=${1:-40}

NL=40

host=$( hostname )
start=$( date +%s )
mode=0

if [ $host = rayol-node-1 ]; then
mode=1
for j in {0..71} ; do
	 ./rho27.exe   -t$NL  -i B-4-6-7.dat -j$j -m144 &> /tmp/rho27-$j.txt &
done
wait
fi
if [ $host = rayol-node-2 ]; then
mode=2
for j in {72..143} ; do
	 ./rho27.exe   -t$NL  -i B-4-6-7.dat -j$j -m144 &> /tmp/rho27-$j.txt &
done
wait
fi

if [ $mode = 0 ]; then
	echo bad host : $host
	exit
fi

file=$host-NL-2-7-$NL.dat
cat /tmp/rho27-*.txt  > $file

ends=$( date +%s )

echo runtime: $(( ends -start  )) >> $file
