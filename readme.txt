=========================================================
determination of covering radii of RM-code in 7 variables
=========================================================

 [ One can use the scripts  rho2.sh and rho3.sh ]

[1] boole at distance >= 41 from RM(2,7) in B(3,4,7) + B(5,5,7)  ?

for j in {0..31};  do ./rho27.exe -t 41 -i B-3-4-7.dat  -j$j -m32 ; done
no soluce

[2] boole at distance  40  from RM(2,7) in B(3,4,7) + B(5,5,7)  ?
for j in {0..31};  do ./rho27.exe -t 40 -i B-3-4-7.dat  -j$j -m32 ; done

They are 4 solutions:

40 anf=acd+bcd+ace+adf+abg+bdg+deg+fg
40 anf=abd+ace+ade+bcf+bdf+cef+abg+abcg+adg+bdg+abdg+aceg+fg+bcfg+defg
40 anf=bcd+ace+bce+acf+abcf+bdf+cdf+aef+acef+cdef+abg+bdg+bceg+deg+fg+bdfg+aefg+befg
40 anf=cde+adf+bdf+bef+bdg+aeg+fg+defg


[3] boole at distance >= 20 from RM(3,7) in in B(4,7,7) ?

We use the probabilist way to filter Boolean at distance >= 20

./rho27.exe -t 20 -i B-4-4-7.dat  -r 1024 > /tmp/step1.txt
#count=1535 best=23

./rho27.exe -t 20 -i /tmp/step1.txt  -r 1024 > /tmp/step2.txt
#count=856 best=22
...
#count=540 best=22
#count=363 best=22
#count=190 best=22
#count=122 best=21
#count=79  best=20   
#count=49 best=20
#count=29 best=20
#count=14 best=20
#count=10 best=20


20 anf=+b+c+bc+d+bd+acd+abcd+e+abe+ace+bce+de+bde+cde+bcde+bdf+cdf+ef+aef+def+cdef+g+ag+bg+abg+cg+acg+bcg+abcg+dg+adg+cdg+eg+ceg+deg+abfg+efg+aefg+defg
20 anf=+b+c+ac+bc+abc+d+bd+abd+e+be+ce+bce+de+ade+bde+af+abf+cf+acf+bcf+abcf+adf+ef+bef+acef+def+cdef+g+bg+adg+bceg+deg+afg+dfg+bdfg+efg+aefg+befg
20 anf=d+ad+bd+cd+bcd+be+abe+ce+bce+de+ade+acde+abcf+bdf+cdf+ef+aef+cef+def+bdef+g+ag+cg+acg+dg+adg+bdg+cdg+bcdg+abeg+ceg+deg+bfg+dfg+adfg+cefg
20 anf=ab+abc+abd+abe+bce+de+bde+cde+bf+bcf+bdf+acdf+bcdf+bef+abcef+def+bdef+ag+bg+abg+acg+bcg+adg+abcdg+eg+ceg+bceg+adeg+fg+abfg+cfg
20 anf=a+b+ab+ac+bc+abc+ad+bd+acd+bcd+ae+be+abe+bce+de+ade+cde+abcde+af+bf+abf+adf+ef+cef+bcef+adef+g+cg+bdg+bcdg+eg+ceg+aceg+deg+fg+bfg+abfg+cfg+efg


At most 5 solutions !

