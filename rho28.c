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
int tour = 0;
FILE *src = NULL;

int job = 0;
int module = 1;

int enumcoset(word * f, int dc, word ** C, int dw, word ** W)
{
    u_int64_t limite = 1;
    limite = limite << dw;
    u_int64_t vec = 1;
    int count = 0;
    int e = estimationWord(f, C, dc, target);
    if (e >= target) {
	count = count + 1;
	printWord(e, f);
    }

    while (vec < limite) {
	int i = __builtin_ctzll(vec);
	f[0] ^= W[i][0];
	f[1] ^= W[i][1];
	e = estimationWord(f, C, dc, target);
	if (e >= target) {
	    count = count + 1;
	    printWord(e, f);
	}
	vec = vec + 1;
    }
    return count;
}

typedef struct list {
    word z[2];
    struct list *next;
} enr, *list;

list append(word z[2], word zf[2], list l)
{
    list aux = calloc(1, sizeof(enr));
    aux->z[0] = z[0]^zf[0];
    aux->z[1] = z[1]^zf[1];
    aux->next = l;
    return aux;
}

void freelist(list * t)
{
    int i;
    list aux;
    for (i = 0; i < ffsize; i++)
	while (t[i]) {
	    aux = t[i];
	    t[i] = aux->next;
	    free(aux);
	}
    free(t);
}

int ranklist(list l)
{
    list aux = NULL;
    int r = 0, k;
    word p;
    while ( l ) {
	if (l->z[0] == 0 && l->z[1] == 0) {
	    l = l->next;
	} else {
	    if (l->z[0]) {
		p = l->z[0] & (l->z[0] - 1);
		p ^= l->z[0];
		k = 0;
	    } else {
		p = l->z[1] & (l->z[1] - 1);
		p ^= l->z[1];
		k = 1;
	    }
	    aux = l->next;
	    while (aux) {
		if ( aux->z[k] & p   ) {
		     aux->z[0] ^= l->z[0];
		     aux->z[1] ^= l->z[1];
		}
		aux = aux->next;
	    }
	    l = l->next;
	    r++;
	}
    }
    return r;
}


void translate(boole f)
{
    int *d = calloc(ffsize, sizeof(int));
    list *l = calloc(ffsize, sizeof(list));
    code c = RM(2, ffdimen);
    word **cz = pack(c);
    u_int64_t limite = 1;
    limite = limite << c.dim;
    u_int64_t vec = 1;
    word z[2], zf[2];
    zip(z, f);
    zip(zf, f);

    int w = WT(z);
    d[w]++;
    while (vec < limite) {
	int i = __builtin_ctzll(vec);
	z[0] ^= cz[i][0];
	z[1] ^= cz[i][1];
	w = WT(z);
	d[w]++;
	if (w < 56)
	    l[w] = append(z, zf, l[w]);
	vec = vec + 1;
    }
    printf("\nd:");
    for (w = 0; w < 56; w++)
	if ( d[w] )
	    printf(" %d [ %d ] --> rank = %d\n", d[w], w, ranklist( l[w] ));
    printf("\n");
    free(d);
    freeCode(c);
    int i;
    for (i = 0; i < c.dim; i++)
	free(cz[i]);
    free(cz);
    freelist(l);
}


void rho28(void)
{
    int val;


    int count = 0;
    boole f;
    int num = 0;
    while ((f = loadBoole(src, &val))) {
	projboole(3, ffdimen, f);
	panfnum(val, f);
	printf("\n");
	translate(f);
	free(f);
	num++;
    }

    printf("\n#count=%d\n", count);
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
	    src = fopen(optarg, "r");
	    break;
	case 'w':
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

    ffdimen = 7;
    ffsize = 128;

    if (!src) {
	perror("filename");
	exit(1);
    }

    srandom(time(NULL));

    rho28();

    printf("#class NL_3 >= %d :", target);

    fclose(src);
    putchar('\n');
    return 0;
}
