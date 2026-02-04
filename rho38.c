#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include "boole.h"
#include "code.h"
#include "time.h"

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

int main(int argc, char *argv[])
{

    int opt;

   
    while ((opt = getopt(argc, argv, "t:o:vij::r:m:")) != -1) {
	switch (opt) {
	case 'r':
	    tour = atoi(optarg);
	    break;
	case 't':
	    target = atoi(optarg);
	    break;
	case 'i':
	    src = fopen( optarg , "r");
	    break;
	case 'm':
	    modulo = atoi( optarg );
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
	exit(1);
    }

    srandom( time(NULL) );

    printf("#class NL_3 >= %d :", target);

    rho38( );
    fclose(src);
    putchar('\n');
    return 0;
}
