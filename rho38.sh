#!/usr/bin/bash

NL=${1:-60}


ROUND=1024
let ROUND*=16

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

echo ./rho38.exe   -t$NL -r$ROUND -i $input  '>'  $output
./rho38.exe   -t$NL -r$ROUND -i $input  >  $output

grep anf $output > NL-3-8-$NL.dat

ends=$( date +%s )

echo runtime: $(( ends -start  ))

wc -l NL-3-8-$NL.dat
