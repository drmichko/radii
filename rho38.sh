#!/usr/bin/bash

NL=${1:-60}

NL=56

ROUND=1024

if [ ! -f /tmp/rho38-0.txt ] ; then
	cp B-4-4-8.dat /tmp/rho38-0.txt
fi

start=$( date +%s )

num=0;

while true; do 
	output=/tmp/rho38-$((num+1)).txt
	if [ ! -f $output ] ; then
        	break
	fi
	let num++
done

input=/tmp/rho38-$num.txt
let num++
output=/tmp/rho38-$num.txt

NPROC=$( nproc )
for(( j = 0; j < NPROC; j++ )); do
        echo -n '.'
        ./rho38.exe   -t$NL  -r$ROUND  -j$j -m$NPROC  -i $input   > /tmp/out-38-$j.txt    &
done

echo
wait

cat   /tmp/out-38-*.txt > $output

rm  -f  /tmp/out-38-*.txt

grep anf $output > NL-3-8-$NL.dat

ends=$( date +%s )

echo runtime: $(( ends -start  ))

wc -l NL-3-8-$NL.dat
