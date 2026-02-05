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

int verb = 0;
int target = 1;
int  tour  = 1;
FILE *src = NULL;
int job = 0;
int modulo = 1;

void rho38( void )
{
    ffdimen = 8;
    ffsize = 256;
    int val, res;
    code c = RM( 3 , ffdimen);
    int count = 0,  num =0;
    boole f;
    int best = 0;
    while (  ( f = loadBoole( src, &val) ) ) {
	if ( job == num % modulo ) {
		if ( val < 0 ) 
			val = wtBoole( f );
		res = probabiliste ( c,  f,  tour , target );
		if (  res ) {
	    	if ( res > val ) res = val; 	
	    	panfnum(res, f);
	    	count++;
	    	if ( res > best ) best = res;
		}
	}
	num++;
    }

    freeCode(c);
    printf("\n#count=%d best=%d\n", count , best );
}



int zdistance (  word** C, int dim_C ){
	u_int64_t limite = 1;
	limite = limite  << dim_C;
	u_int64_t v=1;
	limite = ((u_int64_t)1) << dim_C;
	int i;
	int w;
	int res = ffsize;
	word f[ 2 ]={0,0};

  while (v<limite){
    i = __builtin_ctzll(v);
    f[0] ^= C[i][0];
    f[1] ^= C[i][1];
    w = WT(f);

    if (w<res){
      res = w;
    }
    v =  v + 1;
  }
  return res;
}



int main(int argc, char *argv[])
{

    int k = 2, opt;

 
    while ((opt = getopt(argc, argv, "t:o:vi:j:r:m:k:")) != -1) {
	switch (opt) {
	case 'r':
	    tour = atoi(optarg);
	    break;
	case 'k':
	    k = atoi(optarg);
	    break;
	case 't':
	    target = atoi(optarg);
	    break;
	case 'i':
	    src = fopen( optarg , "r");
	    break;
	case 'm':
		ffdimen = atoi( optarg );
		ffsize  = 1 << ffdimen;
	    break;
	case 'j':
	    job = atoi( optarg );
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
	/// exit(1);
    }

    srandom( time(NULL) );

    code rm =  RM( k, ffdimen );

    word** zrm = pack( rm );

    int dmin = zdistance( zrm, rm.dim   );
    
    freeCode( rm );
    // fclose(src);
    printf("dmin=%d", dmin );
    putchar('\n');
    return 0;
}
