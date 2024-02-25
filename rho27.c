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

int enumcoset( word* f, int dc, word** C, int dw, word** W ){
  u_int64_t limite = 1;
  limite = limite  << dw;
  u_int64_t vec = 1;
  int count = 0;
  int e = estimationWord(f, C, dc, target );
  if ( e >=  target ){
      count = count + 1;
      printWord(e, f );
    }

  while ( vec < limite ) {
    int i = __builtin_ctzll( vec );
    f[0] ^= W[i][0];
    f[1] ^= W[i][1];
    e = estimationWord(f, C, dc, target );
    if ( e >=  target ){
      count = count + 1;
      printWord(e, f );
    }
    vec = vec + 1;
  }
  return count;
}

int probcoset( boole f, code c, code w ){
  u_int64_t limite = 1;
  limite = limite  << w.dim;
  u_int64_t vec = 1;
  int count = 0;
  int e = probabiliste (c , f, tour, target);
  if ( e >=  target ){
      count = count + 1;
      if ( verb ) panfnum( e, f );
    }

  while ( vec < limite ) {
    int i = __builtin_ctzll( vec );
    addBoole( f, w.G[i]   );
    int e = probabiliste (c , f, tour, target);
    if ( e >=  target ){
      count = count + 1;
      if ( verb ) panfnum( e, f );
    }
    vec = vec + 1;
  }
  return count;
}


void rho27( void )
{
    ffdimen = 7;
    ffsize = 128;
    int val;
    
    code c = RM(2, ffdimen);
    code b = B(5,5,7);
    word** cz = pack( c );
    word** bz = pack( b );

    word w[2];

    int count = 0;
    boole f;
    int num = 0;
    int step = 0;
    while (  ( f = loadBoole( src, &val) ) ) {
	if ( val < 0 ) val = ffsize;
	if (  num % module == job ) {
		zip( w, f );
		count += enumcoset( w, c.dim, cz, b.dim, bz  );
                step++;
	}
	free( f );
	num++;
    }

    freeCode(c);
    freeCode(b);
    printf("\n#count=%d\n", count );
}


void prob27( void )
{
    ffdimen = 7;
    ffsize = 128;
    int val;
    
    code c = RM(2, ffdimen);
    code b = B(5,5,7);


    int count = 0;
    boole f;
    int num = 0;
    int step = 0;
    while (  ( f = loadBoole( src, &val) ) ) {
	if ( val < 0 ) val = ffsize;
	if ( num % module == job ){
		count += probcoset(  f , c,  b   );
		printf("%d %8d %d\n", step, num, count );
		step++;
	}
	free( f );
	num++;
    }

    freeCode(c);
    freeCode(b);
    printf("\n#count=%d\n", count );
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

    rho27();

    printf("#class NL_3 >= %d :", target);

    fclose(src);
    putchar('\n');
    return 0;
}
