#!/usr/bin/bash


NL=${1:-40}

host=$( hostname )
start=$( date +%s )
mode=0

if [ $hostname = rayol-node-1 ]; then
mode=1
for j in {0..71} ; do
	time ./rho27.exe   -t$NL  -i B-3-4-7.dat -j$j -m144 &> /tmp/rho27-$j.txt &
done
wait
fi
if [ $hostname = rayol-node-2 ]; then
mode=2
for j in {72..143} ; do
	time ./rho27.exe   -t$NL  -i B-3-4-7.dat -j$j -m144 &> /tmp/rho27-$j.txt &
done
wait
fi

if [ $mode = 0 ]; then
	echo bad host : $host
	exit
fi

file=$host-NL-2-7-$NL.dat
cat /tmp/rho27-*.txt  | grep anf > $file

ends=$( date +%s )

echo runtime: $(( ends -start  )) >> $file
