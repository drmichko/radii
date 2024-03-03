#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include "boole.h"
#include "code.h"
#include "time.h"
#include "word.h"

int optwrite = 0;
int verb = 0;
int target = 1;
int  tour  = 0;
FILE *src = NULL;

int job    = 0;
int module = 1;

code rm;

void Fourier(int *f, unsigned int n )
// Transformation de Fourier sur place.
{
int x,y,z;
if (n>1) {
         Fourier(f,n>>1);
         Fourier(&(f[n>>1]),n>>1);
         for(x=0,y=(n>>1);  x< (n>>1); x++,y++)
                {
                z = f[x];
                f[x] = z + f[y];
                f[y] = z - f[y];
                }
         };
}


int linearity( boole f , int *flag   )
{ int tfr[ ffsize ];
  int x;
  int res = 0, min = 16;
  *flag = 0;
  for( x = 0; x < ffsize; x++ )
          tfr[x] = f[x] ? -1 : +1;
  Fourier( tfr, ffsize );
  for( x = 0; x < ffsize; x++ ){
	  int w = abs( tfr[x] );
         if ( w  >  res )
             res =  abs( tfr[x] );
     	 if ( w && w < min )
	     min = w;
     }
  *flag = min;
  return res;
}

void check( boole f ) 
{ int flag[ 32 ] = {0};
  int min;
  int tfr = linearity( f , & min );
  if ( tfr == 16 ) flag[ min ]++;

  u_int64_t limite = 1;
  limite = limite  << rm.dim;
  u_int64_t vec = 1;

  printANF( f );
  while ( vec < limite ) {
    int i = __builtin_ctzll( vec );
    addBoole( f, rm.G[i]   );
    tfr = linearity( f , & min );
    if ( tfr == 16 ) 
	    flag[ min ]++;
    vec = vec + 1;
  }
  int i;
  for( i = 0; i < 17; i++ )
	   if ( flag[i] ) printf("%d [%d] ", flag[i],  i);
  printf("\n");
}

void rho17( void )
{
    int val;
    boole f;
    int num = 0;
    while (  ( f = loadBoole( src, &val) ) ) {
	if ( val < 0 ) val = ffsize;
	if (  num % module == job ) {
		check( f );
	}
	free( f );
	num++;
    }

}




int main(int argc, char *argv[])
{

    int opt;


    while ((opt = getopt(argc, argv, "t:o:vi:r:j:m:")) != -1) {
	switch (opt) {
	case 'r':
	    tour = atoi(optarg);
	    break;
	case 'j':
	    job = atoi(optarg);
	    break;
	case 'm':
	    module = atoi(optarg);
	    break;
	case 't':
	    target = atoi(optarg);
	    break;
	case 'i':
	    src = fopen( optarg , "r");
	    break;
	case 'w' :
	    optwrite = 1;
	    break;
	case 'v':
	    verb++;
	    break;
	default:		/* '?' */
	    fprintf(stderr, "check usage of %s !!!\n", argv[0]);
	    exit(EXIT_FAILURE);
	}
    }

    if (!src) {
	perror("filename");
	exit(1);
    }

    srandom( time(NULL) );

    ffdimen = 7;
    ffsize  = 128;
    rm = B( 2, 2, ffdimen );
    rho17();


    fclose(src);
    putchar('\n');
    return 0;
}
