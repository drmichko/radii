#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include "boole.h"
#include "code.h"

int verb = 0;
int target = 1;
FILE *src = NULL;

void rho26( void )
{
    ffdimen = 6;
    ffsize = 64;
    int val, res;
    code c = RM(2, ffdimen);

    boole f;
    while (  ( f = loadBoole( src, &val) ) ) {
	if ( val < 0 ) val = ffsize;
	res = estimation(f, c, target);
	if (  res ) {
	    if ( res > val ) res = val; 
    	    projboole(3, ffdimen, f)	    ;
	    panfnum(res, f);
	}
    }

    freeCode(c);
}

int main(int argc, char *argv[])
{

    int opt;


    while ((opt = getopt(argc, argv, "t:o:vi:")) != -1) {
	switch (opt) {
	case 't':
	    target = atoi(optarg);
	    break;
	case 'i':
	    src = fopen( optarg , "r");
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

    printf("#class NL_2 >= %d :", target);

    rho26( );
    fclose(src);
    putchar('\n');
    return 0;
}
