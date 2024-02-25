OPTION = -O3  -march=native  -Wno-unused-result

ifeq ($(shell hostname),localhost.localdomain)
	OPTION = -Wall -g
endif


CFLAGS  = $(OPTION)

all  : rho26.exe rho37.exe rho27.exe



rho26.exe  : boole.o  code.o rho26.c  
	gcc $(CFLAGS) $^ -o $@

rho37.exe  : boole.o  code.o rho37.c  
	gcc $(CFLAGS) $^ -o $@

rho27.exe  : boole.o  code.o word.o rho27.c  
	gcc $(CFLAGS) $^ -o $@
code.o : code.h code.c
	gcc $(CFLAGS) $^ -c

word.o : word.c word.h
	gcc $(CFLAGS) $<  -c

boole.o : boole.h boole.c
	gcc $(CFLAGS) $^ -c

clean :
	rm *.o *.gch

proper :
	rm *.o *.exe *~ 

