#!/usr/bin/bash


NL=${1:-40}
start=$( date +%s )

for j in {0..71} ; do
	time ./rho27.exe   -t$NL  -i B-3-4-7.dat -j$j -m72 &> /tmp/rho27-$j.txt &
done

wait

cat /tmp/rho27-*.txt  | grep anf > NL-2-7-$NL.dat

ends=$( date +%s )

echo runtime: $(( ends -start  ))
wc -l NL-2-7-36.txt
