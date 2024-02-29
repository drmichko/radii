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
  int res = 0;
  *flag = 0;
  for( x = 0; x < ffsize; x++ )
          tfr[x] = f[x] ? -1 : +1;
  Fourier( tfr, ffsize );
  for( x = 0; x < ffsize; x++ ){
     if ( abs( tfr[x] ) >  res )
             res =  abs( tfr[x] );
     if ( tfr[x]  == 0 )
	     *flag = 1;
     }
  return res;
}

void rho18( void )
{
    int val;
    int best = ffsize;


    boole f;
    int num = 0;
    int flag = 0;
    while (  ( f = loadBoole( src, &val) ) ) {
	if ( val < 0 ) val = ffsize;
	if (  num % module == job ) {
		int tfr = linearity( f , & flag );
		if ( flag && tfr < best ) {
			best = tfr;
			printf("best=%d\n", best );
		}
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

    ffdimen = 8;
    ffsize = 256;
    rho18();


    fclose(src);
    putchar('\n');
    return 0;
}
