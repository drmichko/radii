#!/usr/bin/bash

NL=16
ROUND=1000

if [ ! -f /tmp/rho37-0.txt ] ; then
	cp B-4-7-7.dat /tmp/rho37-0.txt
fi

start=$( date +%s )

num=0;

while true; do 
	output=/tmp/rho37-$((num+1)).txt
	if [ ! -f $output ] ; then
        	break
	fi
	let num++
done

input=/tmp/rho37-$num.txt

echo ./rho37.exe   -t$NL -r$ROUND -i $input  '>'  $output
./rho37.exe   -t$NL -r$ROUND -i $input  >  $output

grep anf $output > NL-3-7-16.txt

ends=$( date +%s )

echo runtime: $(( ends -start  ))

wc -l NL-3-7-16.txt
