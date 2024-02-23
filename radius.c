#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include "boole.h"
#include "code.h"

void rho26( void )
{
  ffdimen = 6;
  ffsize  = 64;

  FILE *src = fopen("./class-3-6.txt", "r");
  if (! src ) {
	  perror("rm26");
	  exit(1);
  }

  char ligne[1024];

  int res;
  code c = RM(2, ffdimen);


  while(fgets(ligne,1024,src)){
	boole f = strToBoole(ligne);
	res = estimation(f,c,1);

    if (res){
      printANF(f);
      printf("\nestimation =  %d\n", res);
    }



  }

  freeCode(c);
}

int main(int argc, char const *argv[]) {

int opt = atoi( argv[1] );

if ( opt  == 26 ) 
	rho26(  );
  return 0;
}
